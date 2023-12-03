local chains = 0
function printChain(pre, u)--打印指针 
	if u.offset == nil then
		chains = chains + 1
		return chains..': '..pre..' = '..u.value
	else
		local ret = ''
		for offset, v in pairs(u.offset) do
			--ret = ret..'\n\n'..printChain(pre..string.format(' -> 0x%X + 0x%X', u.value, offset), v)
			ret = ret..'\n\n'..printChain(pre..string.format(' -> + 0x%X', offset), v)
		end
		if ret ~= '' then ret = ret:sub(3) end	
		return ret
	end	
end

local ti = gg.getTargetInfo()
local x64 = ti.x64
if gg.getResultsCount() == 0 then 
	print('搜索列表为空')
	os.exit()
end

function loadChain(lvl, p)--加载指针
	local fix, maxo, lev = not x64, maxOffset, level
	for k = lvl, 1, -1 do
		local levk, p2, stop = lev[k], {}, true
		for j, u in pairs(p) do		
			if u.offset == nil then			
				u.offset = {}
				if fix then u.value = u.value & 0xFFFFFFFF end
				for i, v in ipairs(levk) do
					local offset = v.address - u.value
					if offset >= 0 and offset <= maxo then 
						u.offset[offset], p2[v], stop = v, v, false
					end 
				end
			end
		end
		if stop then break end
		p = p2
	end
end

function getRanges()--取出一级指针内存范围
	local archs = {[0x3] = 'x86', [0x28] = 'ARM', [0x3E] = 'x86-64', [0xB7] = 'AArch64'}
	local ranges = {}
	local t = gg.getRangesList('^/data/*.so*$')
	local arch = 'unknown'
	for i, v in ipairs(t) do
		if v.type:sub(2, 2) == '-' then
			local t = gg.getValues({{address = v.start, flags = gg.TYPE_DWORD}, {address = v.start + 0x12, flags = gg.TYPE_WORD}})--获取地址头
			if t[1].value == 0x464C457F then--判断ELF头
				arch = archs[t[2].value]--ARM
				if arch == nil then arch = 'unknown' end
			end
		end	
		if v.type:sub(2, 2) == 'w' then
			v.arch = arch
			table.insert(ranges, v)
		end
	end
	return ranges
end

function XA(so)--取出首地址头
	local t = gg.getRangesList('^/data/*.so*$')
	for i, v in ipairs(t) do
		if v.state == 'Xa' and string.find(so,v.internalName:gsub('^.*/', '')) then
			local t = gg.getValues({{address = v.start, flags = gg.TYPE_DWORD}, {address = v.start + 0x12, flags = gg.TYPE_WORD}})--获取地址头
			if t[1].value == 0x464C457F then--判断ELF头地址
			return v.internalName,tonumber(t[1].address,10)
			end
		end
	end
end

local ranges = getRanges()--一级指针

gg.setRanges(gg.REGION_C_HEAP | gg.REGION_C_ALLOC | gg.REGION_C_DATA | gg.REGION_C_BSS | gg.REGION_ANONYMOUS|gg.REGION_C_DATA|gg.REGION_CODE_APP)

while true do
	if def == nil then def = {3, 0x100, gg.getFile():gsub('[^/]*$', '').."结果.txt"} end
	gg.alert("请认真阅读以下内容:\n    首先感谢Enyby提供的开源脚本,我在此基础上进行了基址偏移的优化,目前筛选功能暂无(主要是懒)有能力的自行添加,再此致敬Enyby！\n注意事项:\n    1.首次扫描可能会出现Nothing found,别担心,只需要再扫一次就好了,BUG原因不明有能力的自行修复.\n    2.此脚本仅供学习交流,禁止用于商业用途\n\n  --By 堕落天使")
	local p = gg.prompt({'指针级数', '每级最大偏移量(十六进制)','保存路径'}, def, {'number', 'number','file'})
	if p == nil then os.exit() end
	--gg.saveVariable(cfg, cfg_file)--保存xx到路径
	depth = p[1]--指针级数
	maxOffset = tonumber(p[2],16)--转16进制(最大指针偏移)
	local Path = p[3]--保存路径
	level, out = {}, {}
	local old = gg.getResults(100000)--存储指针目标
	for lvl = 0, depth do
		if lvl > 0 then
			local t = gg.getResults(100000)
			level[lvl] = t
			gg.toast(lvl..' from '..depth)
			gg.internal3(maxOffset)
		end
		
		for m, r in ipairs(ranges) do
		--print(string.format("%s ->".."%X -> %X",r.internalName,r.start,r['end']))
			local p = gg.getResults(100000, 0, r.start, r['end'])
			if #p > 0 then
		        gg.removeResults(p)
				loadChain(lvl, p)
				local nr = r.internalName:gsub('^.*/', '')
				local Name,address = XA(nr)
				r.internalName = Name
				r.start = address
				p.map = r
				table.insert(out, p)
			end
		end
		if gg.getResultsCount() == 0 then break	end
	end
	
	gg.loadResults(old)
	
	local chain = ''
	chains = 0
	for j, p in ipairs(out) do
		for i, u in ipairs(p) do
		chain = chain..'\n\n'..
				printChain(string.format('%s + 0x%X = [0x%X]', p.map.internalName:gsub('^.*/', ''), u.address - p.map.start, u.address), u)			
		end
	end
	if chain ~= '' then chain = chain:sub(3) else chain = 'Nothing found' end
	
	p = gg.alert('(共找到:'..chains..' 指针级数:'..depth..' 最大偏移:'..maxOffset..')\n\n'..chain, '保存', '重试', '退出')
	if p == 1 then 
    local f = io.open( Path, 'w+')
    f:write(chain)
    f:close()
	os.exit() end
    if p == 2 then end
	if p == 3 then break end
	end

if #out == 0 then
	print('Nothing found')
	os.exit()
end