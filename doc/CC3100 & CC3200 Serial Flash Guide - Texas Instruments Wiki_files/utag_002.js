//tealium universal tag - utag.4 ut4.0.201608182150, Copyright 2016 Tealium.com Inc. All Rights Reserved.
try{(function(id,loader,u){try{u=utag.o[loader].sender[id]={}}catch(e){u=utag.sender[id]};u.ev={'view':1};u.qsp_delim="&";u.kvp_delim="=";u.qs_delim="?";u.tag_type="script";u.base_url="datacloud.tealiumiq.com/ti/main/16/i.js";u.static_params="jsonp=utag.ut.getvisitorid";u.map={"_random_number":"cb"};u.extend=[function(a,b){if(typeof b['cp.observepoint']!=='undefined'&&b['cp.observepoint']=='yes'){return false}},function(a,b){utag.ut.getvisitorid=function(data){utag.DB("Starting cookie logic");utag_data['cia_troubleshoot']+="2;";if(typeof utag.loader.RC('mytisuccess')!=='undefined'&&utag.loader.RC('mytisuccess')=='1'){utag.loader.SC('utag_main',{'mytisuccess':'1'});utag_data['cp.utag_main_mytisuccessflag']='1';utag_data['cp.utag_main_mytisuccessflag']='1';}
var tiSessionID=utag.data['cp.tiSessionID']||utag.data['cp.utag_main_v_id']||"";var tiChannel=utag.loader.RC("utag_main").channel||"";var tiChannelTime=new Date(utag.loader.RC("utag_main").ctimestamp)||"";var chipset=utag.data['cp.tiPermID']||utag.data['tcPermanentId']||utag.data['cp.chipset']||"";var dcSessionID="";var dcChipset="";var dcChannel="";var dcChannelTime="";if(data.tvt&&data.tvt[0]&&data.tvt[0].t1){utag.DB("Session ID found in Datacloud");dcSessionID=data.tvt[0].t1;}
for(var key in data.tvt){if(!data.tvt.hasOwnProperty(key)){continue;}
if(data.tvt[key].t2){dcChipset=data.tvt[key].t2;}}
if(dcChipset&&dcChipset.length>0){utag.DB("Chipset found in Datacloud:"+dcChipset)};for(var prop in data.tvt){if(!data.tvt.hasOwnProperty(prop)){continue;}
if(data.tvt[prop].t3&&data.tvt[prop].t4){dcChannel=data.tvt[prop].t3;dcChannelTime=data.tvt[prop].t4;}}
if(dcChannel&&dcChannel.length>0){utag.DB("Channel found in Datacloud:"+dcChannel);dcChannelTime=new Date(dcChannelTime);};if(dcSessionID.length>0){tl_setCookie('tiSessionID',dcSessionID,60*24*365*2);utag_data['cp.tiSessionID']=dcSessionID;utag.data['cp.tiSessionID']=dcSessionID;}else{tl_setCookie('tiSessionID',tiSessionID,60*24*365*2);utag_data['cp.tiSessionID']=tiSessionID;utag.data['cp.tiSessionID']=tiSessionID;}
if(dcChipset.length>0&&!chipset){tl_setCookie('chipset',dcChipset,60*24*365*1);utag_data['cp.chipset']=dcChipset;utag.data['cp.chipset']=dcChipset;if(typeof utag.loader.RC('tiPermID')!='object')
utag.loader.SC('tiPermID',{},'da');}else if(chipset.length>0){tl_setCookie('chipset',chipset,60*24*365*1);utag_data['cp.chipset']=chipset;utag.data['cp.chipset']=chipset;if(typeof utag.loader.RC('tiPermID')!='object')
utag.loader.SC('tiPermID',{},'da');}else{utag.DB("chipset doesn't exist")}
if(dcChannel&&dcChannelTime){var server_time_ms=dcChannelTime.getTime();var exp_ms=server_time_ms+2592000000;var current_time=(new Date).getTime();var oneDay=24*60*60*1000;var exp=Math.round((exp_ms-current_time)/(oneDay));if(!tiChannel){utag.DB("Check if server time more than 30 days old");if(exp>0&&exp<=30){utag.loader.SC("utag_main",{"channel":dcChannel+";exp-"+exp+"d","ctimestamp":dcChannelTime+";exp-"+exp+"d"});utag_data['cp.utag_main_channel']=dcChannel;utag_data['cp.utag_main_ctimestamp']=dcChannelTime;utag.data['cp.utag_main_channel']=dcChannel;utag.data['cp.utag_main_ctimestamp']=dcChannelTime;}else if(exp==0){utag.DB("Campaign expires today. Set Session Cookie");utag.loader.SC("utag_main",{"channel":dcChannel+";exp-session","ctimestamp":dcChannelTime+";exp-session"});utag_data['cp.utag_main_channel']=dcChannel;utag_data['cp.utag_main_ctimestamp']=dcChannelTime;utag.data['cp.utag_main_channel']=dcChannel;utag.data['cp.utag_main_ctimestamp']=dcChannelTime;}else{utag.DB("Server timestamp is expired");}}else if(dcChannelTime>tiChannelTime){utag.DB("Server is newer, set local cookies with server data");utag.DB("Check if server time more than 30 days old");if(exp>0&&exp<=30){utag.loader.SC("utag_main",{"channel":dcChannel+";exp-"+exp+"d","ctimestamp":dcChannelTime+";exp-"+exp+"d"});utag_data['cp.utag_main_channel']=dcChannel;utag_data['cp.utag_main_ctimestamp']=dcChannelTime;utag.data['cp.utag_main_channel']=dcChannel;utag.data['cp.utag_main_ctimestamp']=dcChannelTime;}else if(exp==0){utag.DB("Campaign expires today. Set Session Cookie");utag.loader.SC("utag_main",{"channel":dcChannel+";exp-session","ctimestamp":dcChannelTime+";exp-session"});utag_data['cp.utag_main_channel']=dcChannel;utag_data['cp.utag_main_ctimestamp']=dcChannelTime;utag.data['cp.utag_main_channel']=dcChannel;utag.data['cp.utag_main_ctimestamp']=dcChannelTime;}else{utag.DB("Server timestamp is expired");}}else{utag.DB("Server is older than local. Do Nothing");}}
var dcRequestString="";if(utag_data['cp.tiSessionID']){dcRequestString="t1="+utag_data['cp.tiSessionID'];utag_data['cia_troubleshoot']+="3;";if(utag_data['cp.chipset']){dcRequestString+="&t2="+utag_data['cp.chipset'];utag_data['cia_troubleshoot']+="4;";}
if(utag_data['cp.utag_main_channel']&&utag_data['cp.utag_main_ctimestamp']){dcRequestString+="&t3="+utag_data['cp.utag_main_channel']+"&t4="+utag_data['cp.utag_main_ctimestamp'];utag_data['cia_troubleshoot']+="5;";}
utag.ut.loader({type:'img',src:"//datacloud.tealiumiq.com/ti/main/16/i.gif?"+dcRequestString+"&cb="+Math.random()});utag.DB("Saved local cookies to Datacloud");utag.DB(dcRequestString);utag_data['cia_troubleshoot']+="6;";}else{utag.DB("Can't send request to Datacloud without a tiSessionID");}
if(utag_data['tcPageName']&&(utag_data['tcPageName']=='generic_tech_doc_download'||utag_data['tcPageName']=='generic_mkt_collateral_download')){}else{utag.DB("Logic done. Firing GA tag manually.");utag_data['cia_troubleshoot']+="7;";utag.view(utag_data,null,[87]);}};}];u.send=function(a,b){if(u.ev[a]||typeof u.ev.all!="undefined"){for(c=0;c<u.extend.length;c++){try{d=u.extend[c](a,b);if(d==false)return}catch(e){if(typeof utag_err!='undefined'){utag_err.push({e:'extension error:'+e,s:utag.cfg.path+'utag.'+id+'.js',l:c,t:'ex'})}}};var c,d,e,f;c=[];for(d in utag.loader.GV(u.map)){if(typeof b[d]!="undefined"&&b[d]!=""){e=u.map[d].split(",");for(f=0;f<e.length;f++){if(e[f]=="qsp_delim"||e[f]=="kvp_delim"||e[f]=="qs_delim"||e[f]=="base_url"||e[f]=="secure_base_url"){u[e[f]]=b[d];}else{c.push(e[f]+u.kvp_delim+encodeURIComponent(b[d]));}}}}
u.secure_base_url=u.secure_base_url||"datacloud.tealiumiq.com/ti/main/16/i.js"||u.base_url;u.url=(location.protocol=="https:"?u.secure_base_url:u.base_url);if(u.url.indexOf("http")!=0&&u.url.indexOf("/")!=0){u.url="//"+u.url;}
if(u.url.indexOf(u.qs_delim)<0&&(c.length>0||u.static_params.length>0)){u.url+=u.qs_delim}
if(u.static_params){if(c.length>0){u.url+=u.static_params+u.qsp_delim;}else{u.url+=u.static_params;}}
if(u.tag_type=="img"){u.img=new Image();u.img.src=u.url+c.join(u.qsp_delim);}else if(u.tag_type=="script"){u.s=document.getElementsByTagName("script")[0];u.scr=document.createElement("script");u.scr.type="text/javascript";u.scr.src=u.url+c.join(u.qsp_delim);u.s.parentNode.insertBefore(u.scr,u.s);}else{d=document.createElement("iframe");d.setAttribute('id','4');d.setAttribute('height','1');d.setAttribute('width','1');d.setAttribute('style','display:none');d.setAttribute('src',u.url+c.join(u.qsp_delim));document.body.appendChild(d);}}}
try{utag.o[loader].loader.LOAD(id)}catch(e){utag.loader.LOAD(id)}})('4','ti.main');}catch(e){}
