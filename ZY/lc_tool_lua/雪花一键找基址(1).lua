function PS() end
function math.round(int) PS('返回四舍五入后的整数') return math.floor(int + 0.5) end
function search(t,type,name) PS('联合搜索(地址数组[{数值1,{数值2,偏移}...}],内存范围,功能名称[可不填:搜索成功没提示]) 返回搜索结果') rt={} gg.setRanges(type) gg.clearResults()  gg.searchNumber(t[1]..';'..t[2][1]..':'..math.abs(t[2][2])+1, gg.TYPE_DWORD, false, gg.SIGN_EQUAL, 0, -1) local r = gg.getResults(99999999)  if #r==0 then goto last end  for it=2,#t do for i=1,#r do r[i].address=r[i].address+t[it][2] end local rr=gg.getValues(r) tt={} for i=1,#rr do if rr[i].value== t[it][1] then ii=#tt+1 tt[ii]={} tt[ii].address=rr[i].address-t[it][2] tt[ii].flags=4 end end if #tt==0 then goto last end r=gg.getValues(tt) if it==#t then rt=r goto last end end ::last:: if name == nil then else if #rt>0 then gg.toast(name..'修改成功√') else gg.toast(name..'修改失败！未找到符合条件的数据。') end end return rt end
function searchxs(t,type,name) PS('XS快搜(地址数组[{数值1,{数值2,偏移}...}],内存范围,功能名称[可不填:搜索成功没提示]) 返回搜索结果') rt={} gg.setRanges(type) gg.clearResults() gg.searchNumber(t[1], gg.TYPE_DWORD, false, gg.SIGN_EQUAL, 0, -1) local r = gg.getResults(99999999) if #r==0 then goto last end for it=2,#t do for i=1,#r do r[i].address=r[i].address+t[it][2] end local rr=gg.getValues(r) tt={} for i=1,#rr do if rr[i].value== t[it][1] then ii=#tt+1 tt[ii]={} tt[ii].address=rr[i].address-t[it][2] tt[ii].flags=4 end end if #tt==0 then goto last end r=gg.getValues(tt) if it==#t then rt=r goto last end end ::last:: if name == nil then else if #rt>0 then gg.toast(name..'修改成功√') else gg.toast(name..'修改失败！未找到符合条件的数据。') end end return rt end
f1="function searchxs(t,type,name) PS('XS快搜(地址数组[{数值1,{数值2,偏移}...}],内存范围,功能名称[可不填:搜索成功没提示]) 返回搜索结果') rt={} gg.setRanges(type) gg.clearResults() gg.searchNumber(t[1], gg.TYPE_DWORD, false, gg.SIGN_EQUAL, 0, -1) local r = gg.getResults(99999999) if #r==0 then goto last end for it=2,#t do for i=1,#r do r[i].address=r[i].address+t[it][2] end local rr=gg.getValues(r) tt={} for i=1,#rr do if rr[i].value== t[it][1] then ii=#tt+1 tt[ii]={} tt[ii].address=rr[i].address-t[it][2] tt[ii].flags=4 end end if #tt==0 then goto last end r=gg.getValues(tt) if it==#t then rt=r goto last end end ::last:: if name == nil then else if #rt>0 then gg.toast(name..'修改成功√') else gg.toast(name..'修改失败！未找到符合条件的数据。') end end return rt end"
f2="function setvalue(address,flags,value) PS('修改地址数值(地址,数值类型,要修改的值)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags tt[1].value=value gg.setValues(tt) end"
f3="function readvalue(address,flags) PS('查看地址数值(地址,数值类型)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags tt=gg.getValues(tt) return tt[1].value end\nfunction savevalue(address,flags,name) PS('保存地址(地址,数值类型,保存列表项名称)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags if name==nil then else tt[1].name=name end gg.addListItems(tt) end"
f4="function lockvalue(address,flags,value,name) local t = {} t[1] = {} t[1].address = address t[1].flags = flags t[1].value = value t[1].freeze=true t[1].name=name gg.addListItems(t) end"
function setvalue(address,flags,value) PS('修改地址数值(地址,数值类型,要修改的值)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags tt[1].value=value gg.setValues(tt) end
function readvalue(address,flags) PS('查看地址数值(地址,数值类型)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags tt=gg.getValues(tt) return tt[1].value end
function savevalue(address,flags,name) PS('保存地址(地址,数值类型,保存列表项名称)') local tt={} tt[1]={} tt[1].address=address tt[1].flags=flags if name==nil then else tt[1].name=name end gg.addListItems(tt) end
random={}
function random.english(long) PS('返回随机英文字符(长度)') local result='' for i=1,long do local a=math.random(65,122) while(a>=91 and a<=96) do a=math.random(65,122) end result=result..string.char(a) end return result end
function random.number(long) PS('返回随机数字(长度)') local result="" for i=1,long/19+1 do result=result..tostring(math.random(1000000000000000000,9999999999999999999)) end return string.sub(result,1,long) end
file={} 
function file.read(filename) PS('读取文件(文件路径)') local id =io.open(filename, "r") if id==nil then return '' end local result=id:read('*a') id:close() return result end
function file.write(filename,text) PS('写入文件(文件路径,内容)') local id =io.open(filename, "w") if id==nil then return false end local result=id:write(text) id:close() return true end
function file.live(filename) PS('文件是否存在(文件路径) 有返回true 无返回false') if os.rename(filename,filename)==true then return true else return false end end
function file.rename(filename,change) PS('文件重命名/移动') if os.rename(filename,change)==true then return true else return false end end
function file.path(filename) PS('返回文件路径') return string.match(filename, "(.+)/[^/]*%.%w+$")..'/' end
function string.middle(text,one,two) PS('取中间文本(文本,文本1,文本2)') local x=string.find(text,one) local y=string.find(text,two) if x==nil or y==nil then return '' end return string.sub(text,x+#one,y-1) end
function string.split(szFullString, szSeparator) PS('分割文本(文本,分割字符)') local nFindStartIndex = 1 local nSplitIndex = 1 local nSplitArray = {} while true do     local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)     if not nFindLastIndex then      nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))      break     end     nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)     nFindStartIndex = nFindLastIndex + string.len(szSeparator)     nSplitIndex = nSplitIndex + 1  end  return nSplitArray  end
function string.replace(text,replace,to) PS('替换文本(文本,要替换的内容,替换后的内容)') if replace=='.' or replace==',' or replace==')' or replace=='(' or to=='}' or replace=='{' then replace='%'..replace end return string.gsub(text,replace,to) end
--function int(a) return 4294967296+a end
function name(a) if a==nil then return " " end return a end
function int(a) if ce64 or a>0 then return a end local b=4294967296+a if b>0xFFFFFF then b=a16(b) b=tonumber('0x'..string.sub(b,string.len(b)-7,string.len(b))) end if b==nil then b=0 end return b  end
function mk1(so) local a=gg.getRangesList(so) if #a>0 then return(a[1].start) end return 0 end   
function mk2(so) local a=gg.getRangesList(so) if #a>0 then return((a[#a]['end'])) end return 0 end   
function split(szFullString, szSeparator) local nFindStartIndex = 1 local nSplitIndex = 1 local nSplitArray = {} while true do    local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)    if not nFindLastIndex then     nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))     break    end    nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)    nFindStartIndex = nFindLastIndex + string.len(szSeparator)    nSplitIndex = nSplitIndex + 1 end return nSplitArray end
function getint(a) return int(readvalue(a,4)) end
function a16(a) return string.format("%#x",a) end
function dz() local lb=gg.getListItems() for i=1,#lb do if lb[i].freeze then return lb[i].address end end return 0 end
function getFW(cs1) if cs1=='Jh' then return gg.REGION_JAVA_HEAP end if cs1=='Ch' then return gg.REGION_C_HEAP end if cs1=='Ca' then return gg.REGION_C_ALLOC end if cs1=='Cd' then return gg.REGION_C_DATA end if cs1=='Cb' then return gg.REGION_C_BSS end if cs1=='PS' then return gg.REGION_PPSSPP end if cs1=='A'  then return gg.REGION_ANONYMOUS end if cs1=='J'  then return gg.REGION_JAVA end if cs1=='S'  then return gg.REGION_STACK end if cs1=='As' then return gg.REGION_ASHMEM end if cs1=='V'  then return gg.REGION_VIDEO end if cs1=='O'  then return gg.REGION_OTHER end if cs1=='B'  then return gg.REGION_BAD end if cs1=='Xa' then return gg.REGION_CODE_APP end if cs1=='Xs' then return gg.REGION_CODE_SYS end return 0 end

-- 作者：By 雪花
-- 申明：该脚本不支持多级指针扫描，搜索效率跟不上
-- 只支持64位游戏指针扫描，由于现在游戏都是64位，
-- 就不浪费时间专门写个32位游戏的指针扫描了

--矩阵 libUE4so+0xACB5200)+0x20)+0x270 ;
--视频演示用：当前矩阵动态地址：73867E8B30

--第一个数组成员 libUE4so+0xACDB8C0)+0x30)+0xa0)+0x0 ;
--视频演示用：当前第一个数组成员动态地址：72FE1EA080

str=gg.prompt({'查找地址','指针偏移','搜索级数','模块名称'},{'0x','0xFF','2','libUE4.so:bss'},{'number','number','number','text'})
dz=tonumber(str[1])
gg.clearResults()
fw=tonumber(str[2])
js=tonumber(str[3])
gg.searchNumber((dz-fw).."~"..(dz), gg.TYPE_QWORD, false, gg.SIGN_EQUAL, 0, -1)
r = gg.getResults(99999999)
so=gg.getRangesList(str[4])
mk1=so[1].start --模块地址
mk2=so[#so].start --模块末尾
jg={} n=0
jg[1]={}
 for i=1,#r do
    if(r[i].address%4==0) then
        n=n+1
        jg[1][n]={}
        jg[1][n].dz=r[i].address
        jg[1][n].py=dz-r[i].value
        jg[1][n].pyz=a16(jg[1][n].py)
        --print(a16(dz-r[i].value))
    end
 end

for ji=1,js-1 do

if ji~=js-1 then
  n=0
  jg[ji+1]={}
  for i=1,#jg[ji] do
   dz=jg[ji][i].dz
    gg.clearResults()
    gg.searchNumber((dz-fw).."~"..(dz), gg.TYPE_QWORD, false, gg.SIGN_EQUAL, 0, -1)
    r = gg.getResults(99999999)
    for i2=1,#r do
      if(r[i2].address%4==0) then
        n=n+1
        jg[ji+1][n]={}
        jg[ji+1][n].dz=r[i2].address
        jg[ji+1][n].py=dz-r[i2].value
        jg[ji+1][n].pyz=a16(jg[ji+1][n].py)..")+0x"..(jg[ji][i].pyz)
      end
    end
  end
else
 for i=1,#jg[ji] do
  gg.clearResults() gg.setRanges(gg.REGION_C_BSS)
  dz=jg[ji][i].dz
  gg.searchNumber((dz-fw).."~"..(dz), gg.TYPE_QWORD, false, gg.SIGN_EQUAL, mk1, mk2)
  r = gg.getResults(99999999)
  for i2=1,#r do
    if(r[i2].address%4==0) then
      print("libUE4.so+0x"..a16(r[i2].address-mk1)..")+0x"..a16(dz-r[i2].value)..")+0x"..(jg[ji][i].pyz))
    end
  end
 end
end

end
--print(jg)