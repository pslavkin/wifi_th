//tealium universal tag - utag.103 ut4.0.201610271920, Copyright 2016 Tealium.com Inc. All Rights Reserved.
if(typeof JSON!=='object'){JSON={};}
(function(){'use strict';var rx_one=/^[\],:{}\s]*$/,rx_two=/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,rx_three=/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,rx_four=/(?:^|:|,)(?:\s*\[)+/g,rx_escapable=/[\\\"\u0000-\u001f\u007f-\u009f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,rx_dangerous=/[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g;function f(n){return n<10?'0'+n:n;}
function this_value(){return this.valueOf();}
if(typeof Date.prototype.toJSON!=='function'){Date.prototype.toJSON=function(){return isFinite(this.valueOf())?this.getUTCFullYear()+'-'+
f(this.getUTCMonth()+1)+'-'+f(this.getUTCDate())+'T'+f(this.getUTCHours())+':'+f(this.getUTCMinutes())+':'+
f(this.getUTCSeconds())+'Z':null;};Boolean.prototype.toJSON=this_value;Number.prototype.toJSON=this_value;String.prototype.toJSON=this_value;}
var gap,indent,meta,rep;function quote(string){rx_escapable.lastIndex=0;return rx_escapable.test(string)?'"'+string.replace(rx_escapable,function(a){var c=meta[a];return typeof c==='string'?c:'\\u'+('0000'+a.charCodeAt(0).toString(16)).slice(-4);})+'"':'"'+string+'"';}
function str(key,holder){var i,k,v,length,mind=gap,partial,value=holder[key];if(value&&typeof value==='object'&&typeof value.toJSON==='function'){value=value.toJSON(key);}
if(typeof rep==='function'){value=rep.call(holder,key,value);}
switch(typeof value){case'string':return quote(value);case'number':return isFinite(value)?String(value):'null';case'boolean':case'null':return String(value);case'object':if(!value){return'null';}
gap+=indent;partial=[];if(Object.prototype.toString.apply(value)==='[object Array]'){length=value.length;for(i=0;i<length;i+=1){partial[i]=str(i,value)||'null';}
v=partial.length===0?'[]':gap?'[\n'+gap+partial.join(',\n'+gap)+'\n'+mind+']':'['+partial.join(',')+']';gap=mind;return v;}
if(rep&&typeof rep==='object'){length=rep.length;for(i=0;i<length;i+=1){if(typeof rep[i]==='string'){k=rep[i];v=str(k,value);if(v){partial.push(quote(k)+(gap?': ':':')+v);}}}}else{for(k in value){if(Object.prototype.hasOwnProperty.call(value,k)){v=str(k,value);if(v){partial.push(quote(k)+(gap?': ':':')+v);}}}}
v=partial.length===0?'{}':gap?'{\n'+gap+partial.join(',\n'+gap)+'\n'+mind+'}':'{'+partial.join(',')+'}';gap=mind;return v;}}
if(typeof JSON.stringify!=='function'){meta={'\b':'\\b','\t':'\\t','\n':'\\n','\f':'\\f','\r':'\\r','"':'\\"','\\':'\\\\'};JSON.stringify=function(value,replacer,space){var i;gap='';indent='';if(typeof space==='number'){for(i=0;i<space;i+=1){indent+=' ';}}else if(typeof space==='string'){indent=space;}
rep=replacer;if(replacer&&typeof replacer!=='function'&&(typeof replacer!=='object'||typeof replacer.length!=='number')){throw new Error('JSON.stringify');}
return str('',{'':value});};}}());try{(function(id,loader,u){try{u=utag.o[loader].sender[id]={}}catch(e){u=utag.sender[id]};u.ev={'all':1};u.server_domain="tealiumiq.com";u.server_prefix="";u.tag_config_server="//datacloud.tealiumiq.com/ti/main/2/i.gif";u.tag_config_sampling="100"||"100";if(utag.cfg.utagdb){u.tag_config_sampling="100";}
u.tag_config_region="default"||"default";u.region="";u.performance_timing_count=0;u.account=utag.cfg.utid.split("/")[0];u.profile=""||utag.cfg.utid.split("/")[1];if(u.tag_config_server.indexOf("-collect."+u.server_domain)>1){u.server_prefix=u.tag_config_server.split("collect."+u.server_domain)[0];if(u.tag_config_server.indexOf("/i.gif")<0){u.tag_config_server="https://"+[u.server_prefix+"collect."+u.server_domain,u.account,u.profile,"2","i.gif"].join("/");}}
if(u.tag_config_server===""){if(u.tag_config_region==="default"){u.tag_config_server="https://"+[u.server_prefix+"collect."+u.server_domain,u.account,u.profile,"2","i.gif"].join("/");}else{u.tag_config_server="https://"+[u.server_prefix+"collect-"+u.tag_config_region+"."+u.server_domain,u.account,u.profile,"2","i.gif"].join("/");}}
if(u.tag_config_region!=="default"&&u.tag_config_server.indexOf(u.server_prefix+"collect."+u.server_domain)>0){u.tag_config_server=u.tag_config_server.replace(u.server_prefix+"collect."+u.server_domain,u.server_prefix+"collect-"+u.tag_config_region+"."+u.server_domain);u.region=u.tag_config_region;}
u.data_enrichment="none";u.profile_specific_vid=0;u.enrichment_polling=1;u.enrichment_polling_delay=1000;u.do_enrichment=function(){};var q=u.tag_config_server.indexOf("?");if(q>0&&(q+1)==u.tag_config_server.length){u.tag_config_server=u.tag_config_server.replace("?","");}
u.server_list=u.tag_config_server.split("|");u.enrichment_enabled={};u.get_account_profile=function(s){var p=s.substring(s.indexOf(u.server_domain)).split("/");return p;}
u.is_in_sample_group=function(b){var group="100";if(u.tag_config_sampling===""||u.tag_config_sampling==="100"){return true}
if(b["cp.utag_main_dc_group"]){group=b["cp.utag_main_dc_group"];}else{group=Math.floor(Math.random()*100)+1;utag.loader.SC("utag_main",{"dc_group":group});}
if(parseInt(group)<=parseInt(u.tag_config_sampling)){return true}else{return false}}
u.get_performance_timing=function(b){var t,timing;var data={};function subtract(val1,val2){var difference=0;if(val1>val2){difference=val1-val2;}
return difference;}
if(typeof localStorage!="undefined"&&JSON.parse&&window.performance&&window.performance.timing){t=window.performance.timing;timing=localStorage.getItem("tealium_timing");if(timing!==null&&timing!=="{}"&&typeof b!=="undefined"&&u.performance_timing_count===0){utag.ut.merge(b,utag.ut.flatten({timing:JSON.parse(timing)}),1);}}else{return;}
u.performance_timing_count++;for(var k in t){if((k.indexOf("dom")===0||k.indexOf("load")===0)&&t[k]===0&&u.performance_timing_count<20){setTimeout(u.get_performance_timing,1000);}}
data["domain"]=location.hostname+"";data["pathname"]=location.pathname+"";data["query_string"]=(""+location.search).substring(1);data["timestamp"]=(new Date()).getTime();data["dns"]=subtract(t.domainLookupEnd,t.domainLookupStart);data["connect"]=subtract(t.connectEnd,t.connectStart);data["response"]=subtract(t.responseEnd,t.responseStart);data["dom_loading_to_interactive"]=subtract(t.domInteractive,t.domLoading);data["dom_interactive_to_complete"]=subtract(t.domComplete,t.domInteractive);data["load"]=subtract(t.loadEventEnd,t.loadEventStart);data["time_to_first_byte"]=subtract(t.responseStart,t.connectEnd);data["front_end"]=subtract(t.loadEventStart,t.responseEnd);data["fetch_to_response"]=subtract(t.responseStart,t.fetchStart);data["fetch_to_complete"]=subtract(t.domComplete,t.fetchStart);data["fetch_to_interactive"]=subtract(t.domInteractive,t.fetchStart);try{localStorage.setItem("tealium_timing",JSON.stringify(data));}catch(e){utag.DB(e)}}
u.map={};u.extend=[function(a,b){var sites=[{name:'dev',property:'UA-64522585-2',domainMatches:[/\-uat/,/\-sat/,/\-int/,/tiportal/,/sectest1/,/infolinkuat/,/focus.*(dev|stage)/],domains:['cmtest.sc.ti.com','commerce-test.itg.ti.com','dlev291.itg.ti.com','dlev407.itg.ti.com','dlev408.itg.ti.com','dlev409.itg.ti.com','dlev410.itg.ti.com','dlev411.itg.ti.com','e2e-uat.ti.com','e2e55-uat.itg.ti.com','e2e60-uat.itg.ti.com','e2ecn-uat.ti.com','e2eDeyi-uat.itg.ti.com','e2ePC-uat.itg.ti.com','e2eprivate-uat.ti.com','emkdev-test.sc.ti.com','focus-dev-cn.sc.ti.com','focus-dev-jp.sc.ti.com','focus-dev.sc.ti.com','focus-stage-cn.ext.ti.com','focus-stage-jp.ext.ti.com','focus-stage.ext.ti.com','focus-webapps-dev-cn.sc.ti.com','focus-webapps-dev-jp.sc.ti.com','focus-webapps-dev.sc.ti.com','localhost','m-cn-int.itg.ti.com','m-cn-sat.itg.ti.com','m-cn-uat.itg.ti.com','m-dev.itg.ti.com','m-int.itg.ti.com','m-jp-int.itg.ti.com','m-jp-sat.itg.ti.com','m-jp-uat.itg.ti.com','m-sat.itg.ti.com','m-uat.itg.ti.com','m2-int.itg.ti.com','m2-uat.itg.ti.com','myinfolink-dev.ext.ti.com','infolinkuat.ti.com','sectest1.itg.ti.com','ti.kenexastaging.com','ti.wawstaging.com','tiportal-url1.itg.ti.com','trainingcenter-dev.ti.com','uat.designsomething.org','uat.ti-estore.harte-hanks.com','webcontent.itg.ti.com','www-cn-int.itg.ti.com','www-cn-sat.itg.ti.com','www-cn-uat.itg.ti.com','www-int.itg.ti.com','www-int2.itg.ti.com','www-jp-int.itg.ti.com','www-jp-sat.itg.ti.com','www-jp-uat.itg.ti.com','www-sat.itg.ti.com','www-uat.itg.ti.com','www-uat2.itg.ti.com','wwwtest.itg.ti.com']},{name:'non-SC',property:'UA-64522585-3',domainMatches:[/\.sc/,/\.itg/,/make/,/design/,/careers/,/brassring/],domains:['careers.ti.com','xjobs.brassring.com','4com.itg.ti.com','analogeda.itg.ti.com','artisan.dal.design.ti.com','atd','insideatd','baguio.itg.ti.com','corpcom.itg.ti.com','dbumpsite.itg.ti.com','dd6s9.d6.sc.ti.com','dd6s9.ti.com','designflow.itg.ti.com','dfdb.dfab.sc.ti.com','digital.itg.ti.com','dmos2.de.sc.ti.com','dmos5','edaglobe.design.ti.com','edm','euroweb.itg.ti.com','from.itg.ti.com','iiasite.itg.ti.com','infolink.sc.ti.com','infolink.ti.com','inthecloud.ti.com','itsecurity.ti.com','klabs.itg.ti.com','leadingedge2012.itg.ti.com','mobile.ti.com','myinfolink.ti.com','packaging.sc.ti.com','playbook2.itg.ti.com','plus1site.itg.ti.com','quality.sc.ti.com','rfab','security.ti.com','sps01.itg.ti.com','sps04.itg.ti.com','sps05.itg.ti.com','sps07.itg.ti.com','tilaw.itg.ti.com','tim.itg.ti.com','titl.itg.ti.com','tmg.ti.com','turnkey.dal.design.ti.com','voice1.itg.ti.com','webtools.itg.ti.com','workanywhere.ti.com','www.dfab.sc.ti.com','www.ticl.make.ti.com','jobs.ti.com','investor.ti.com','itsecuritypolicy.ti.com','career.ti.com','wwf.web.ti.com']},{name:'prod',property:'UA-64522585-1',domainMatches:[],domains:['commerce.ti.com','e2e.ti.com','e2eprivate.ti.com','focus-webapps.ti.com.cn','focus-webapps.tij.co.jp','focus.ti.com.cn','focus.tij.co.jp','m.ti.com','m.ti.com.cn','m.tij.co.jp','newscenter.ti.com','trainingcenter.ti.com','training.eeworld.com.cn','www.designsomething.org','www.deyisupport.com','store.ti.com','www.ti.com.cn','www.tij.co.jp','dev.ti.com','software-dl.ti.com','downloads.ti.com','myportal.ti.com','processors.wiki.ti.com','ti-pass.ext.ti.com','www.ti.com','training.ti.com','focus.ti.com','webench.ti.com','edu.21ic.com','www-k.ext.ti.com','newscenter-jp.ti.com','www.ti.com.tw','focus-webapps.ti.com','wiki.tiprocessors.com']}];var host=b['tcHostName']||utag_data['tcHostName'];for(var i=0;i<sites.length;i++){for(var j=0;j<sites[i].domains.length;j++){if(sites[i].domains[j]==host){b.ga_account=sites[i].property+',UA-64522585-4';b.ga_tracker='ga_main_tracker, ga_cia_tracker';break;}}
if(!b.ga_account){for(var j=0;j<sites[i].domainMatches.length;j++){if(host.match(sites[i].domainMatches[j])!==null){b.ga_account=sites[i].property+',UA-64522585-4';b.ga_tracker='ga_main_tracker, ga_cia_tracker';console.log(sites[i].domainMatches[j]);break;}}}
if(b.ga_account){break;}}
if(!b.ga_account){b.ga_account='UA-64522585-2,UA-64522585-4';b.ga_tracker='ga_main_tracker, ga_cia_tracker';}},function(a,b){var eventMap={'view':{'datasheet view':{metric:'2'},'internal searches':{metric:'7'},'internal search result clicks':{metric:'8'},'product folder views':{metric:'9'},'sw folder views':{metric:'10'},'tool folder views':{metric:'11'},'solution folder views':{metric:'12'},'3p tool folder views':{metric:'13'},'reference design folder view':{metric:'14'},'community views':{metric:'17'},'high volume orders':{metric:'28'},'sample orders':{metric:'30'},'tool orders':{metric:'32'},'paid ic orders':{metric:'33'},'conversion funnel':{metric:'35'},'external campaign response':{metric:'39'},'internal campaign response':{metric:'40'},'portal page view':{metric:'41'}},'link':{'custom link track':{category:'Links',action:'Custom Link Click',label:b['link_id'],value:'1'},'null search':{metric:'7',category:'Search',action:'Null Search',label:b['link_text'],value:'1'},'reference design search':{metric:'7',category:'Search',action:'Results',label:'Reference Design'
+(b['search_type']&&b['search_type']=='tidesigns-power'?' Power':''),non_interaction:b['event_non_interaction'],value:'1'},'search type':{metric:'7',category:'Search',action:'Results',label:b['search_type']?b['search_type']
.substring(b['search_type'].indexOf(':')+1):'',value:'1'},'data converter':{category:'Learning Center',action:'Data Converter Click',label:b['oct_dc_topicType'],value:'1'},'companion parts':{category:'Companion Product',action:'Companion Product Click',label:b['oct_cp_compGPN'],value:'1'},'file downloads':{metric:'1',category:'Download',action:'Direct',label:b['download_file_name'],value:'1'},'activity promo impression':{metric:'15',category:'Promo',action:'Activity Bar Impression',label:b['activityPromoID'],non_interaction:'1',value:'1'},'homepage promo impression':{metric:'15',category:'Promo',action:'Homepage Impression',label:b['homepagePromoID'],non_interaction:'1',value:'1'},'product promo impression':{metric:'15',category:'Promo',action:'Product Impression',label:b['productPromoID'],non_interaction:'1',value:'1'},'promo click':{metric:'16',category:b['event_category'],action:b['event_action'],label:b['promo_id'],value:'1'},'community participation':{metric:'18',category:'Community',action:'Participation',label:b['community_participation'],value:'1'},'myti page create account':{metric:'19',category:'myTI',action:'Register',label:'Page',value:'1'},'myti page existing login':{metric:'20',category:'myTI',action:'Login',label:'Page',value:'1'},'myti page email validated':{metric:'21',category:'myTI',action:'Validate',label:'Page',non_interaction:'1',value:'1'},'myti short form create account':{metric:'22',category:'myTI',action:'Register',label:'Short Form',value:'1'},'myti short form existing login':{metric:'23',category:'myTI',action:'Login',label:'Short Form',value:'1'},'myti short form email validated':{metric:'24',category:'myTI',action:'Validate',label:'Short Form',non_interaction:'1',value:'1'},'myti long form create account':{metric:'25',category:'myTI',action:'Register',label:'Long Form',value:'1'},'myti long form existing login':{metric:'26',category:'myTI',action:'Login',label:'Long Form',value:'1'},'myti long form email validated':{metric:'27',category:'myTI',action:'Validate',label:'Long Form',non_interaction:'1',value:'1'},'sampleorbuy checkout':{metric:'29',category:'Store',action:'Request',label:b['sampleCheckoutType'],value:'1'},'webench entry':{metric:'31',category:'Design Tools',action:'WEBENCH Entry',label:b['link_id'],value:'1'},'parts in carts':{metric:'34',category:'Distributors',action:'Parts in Carts Buy',label:b['product_id'],value:'1'},'ti store buy':{metric:'29',category:'store',action:'request',label:'buy - '+b['sampleCheckoutType'],value:'1'},'conversion funnel':{metric:'35'},'video start':{metric:'36',category:'Video',action:'Video Start',label:b['video_name'],value:'1'},'video complete':{metric:'37',category:'Video',action:'Video Complete',label:b['video_name'],value:'1'},'page timing':{metric:'38',category:'timing',action:'timing hit',label:'complete page load',non_interaction:'1',value:'1'},'page unload':{metric:'46',category:'timing',action:'timing hit',label:'page unload',non_interaction:'1',value:'1'},'video impression':{category:'Video',action:'Video Impression',label:b['video_name'],non_interaction:'1',value:'1'}}};if(typeof b.events!=='undefined'&&b.events.length>0){var e=b.events||[];for(var x in eventMap){if(b['ut.event']==x){var metricType=(x=='link'?x:'page'+x);for(var i=0;i<e.length;i++){for(var j in eventMap[x]){if(j==e[i]){if(eventMap[x][j].metric){u.data[metricType+'-metric'
+eventMap[x][j].metric]=1;}
if(eventMap[x][j].category){b['event_category']=eventMap[x][j].category
.toLowerCase();}
if(eventMap[x][j].action){b['event_action']=eventMap[x][j].action
.toLowerCase();}
if(eventMap[x][j].label){b['event_label']=eventMap[x][j].label
.toLowerCase();}
if(eventMap[x][j].category||eventMap[x][j].action||eventMap[x][j].label){b['event_non_interaction']=eventMap[x][j].non_interaction||false;b['event_value']=eventMap[x][j].value||'1';}}}}}}}}];u.send=function(a,b,c,d,e,f){if(u.ev[a]||typeof u.ev["all"]!="undefined"){u.make_enrichment_request=false;for(c=0;c<u.extend.length;c++){try{d=u.extend[c](a,b);if(d==false)return}catch(e){if(typeof utag_err!='undefined'){utag_err.push({e:'extension error:'+e,s:utag.cfg.path+'utag.'+id+'.js',l:c,t:'ex'})}}};if(!u.is_in_sample_group(b)){return false}
u.get_performance_timing(b);for(var i=0;i<u.server_list.length;i++){if(b["cp.utag_main_dc_region"]){u.region=b["cp.utag_main_dc_region"];u.server_list[i]=u.server_list[i].replace("datacloud.","datacloud-"+u.region+".");u.server_list[i]=u.server_list[i].replace("collect.","collect-"+u.region+".");}
if(u.enrichment_enabled[i]!==false){u.enrichment_enabled[u.server_list[i]]=true}}
if(u.server_list.length>1){u.profile_specific_vid=1;}
u.data=utag.datacloud||{};u.data["loader.cfg"]={};for(d in utag.loader.GV(utag.loader.cfg)){if(utag.loader.cfg[d].load&&utag.loader.cfg[d].send){utag.loader.cfg[d].executed=1;}else{utag.loader.cfg[d].executed=0;}
u.data["loader.cfg"][d]=utag.loader.GV(utag.loader.cfg[d]);}
u.data.data=b;for(d in u.data.data){if((d+'').indexOf("qp.")==0){u.data.data[d]=encodeURIComponent(u.data.data[d]);}else if((d+'').indexOf("va.")==0){delete u.data.data[d]}}
if(!b["cp.utag_main_dc_event"]){b["cp.utag_main_dc_visit"]=(1+(b["cp.utag_main_dc_visit"]?parseInt(b["cp.utag_main_dc_visit"]):0))+'';}
b["cp.utag_main_dc_event"]=(1+(b["cp.utag_main_dc_event"]?parseInt(b["cp.utag_main_dc_event"]):0))+'';utag.loader.SC("utag_main",{"dc_visit":b["cp.utag_main_dc_visit"],"dc_event":b["cp.utag_main_dc_event"]+";exp-session"});utag.data["cp.utag_main_dc_visit"]=b["cp.utag_main_dc_visit"];utag.data["cp.utag_main_dc_event"]=b["cp.utag_main_dc_event"];var dt=new Date();u.data.browser={};try{u.data.browser["height"]=window.innerHeight||document.body.clientHeight;u.data.browser["width"]=window.innerWidth||document.body.clientWidth;u.data.browser["screen_height"]=screen.height;u.data.browser["screen_width"]=screen.width;u.data.browser["timezone_offset"]=dt.getTimezoneOffset();}catch(e){utag.DB(e)}
u.data["event"]=a+'';u.data["post_time"]=dt.getTime();if(u.data_enrichment=="frequent"||u.data_enrichment=="infrequent"){u.visit_num=b["cp.utag_main_dc_visit"];if(parseInt(u.visit_num)>1&&b["cp.utag_main_dc_event"]=="1"){u.enrichment_polling=2;}
try{u.va_update=parseInt(localStorage.getItem("tealium_va_update")||0);}catch(e){utag.DB(e)}
u.visitor_id=u.visitor_id||b["cp.utag_main_v_id"];if((u.data_enrichment=="frequent"&&!(u.visit_num=="1"&&b["cp.utag_main_dc_event"]=="1"))||(u.data_enrichment=="infrequent"&&parseInt(u.visit_num)>1&&parseInt(b["cp.utag_main_dc_event"])<=5&&u.visit_num!=u.va_update)){u.make_enrichment_request=true;}else if(b._corder){u.make_enrichment_request=true;u.enrichment_polling=3;u.enrichment_polling_delay=4000;}
u.visitor_service_request=function(t,server){var s="https://"+u.server_prefix+"visitor-service"+(u.region?"-"+u.region:"")+"."+u.server_domain;var p=u.get_account_profile(server);(function(p){var prefix="tealium_va";var key="_"+p[1]+"_"+p[2];utag.ut["writeva"+p[2]]=function(o){utag.DB("Visitor Attributes: "+prefix+key);utag.DB(o)
var str=JSON.stringify(o);if(str!="{}"&&str!=""){try{localStorage.setItem('tealium_va_update',utag.data["cp.utag_main_dc_visit"]);localStorage.setItem(prefix,str);localStorage.setItem(prefix+key,str);}catch(e){utag.DB(e)}
if(typeof tealium_enrichment=="function"){tealium_enrichment(o,prefix+key);}}}}(p.slice(0)))
var vid=u.visitor_id;if(u.profile_specific_vid==1){vid+=p[2];}
utag.ut.loader({id:"tealium_visitor_service_103"+p[2],src:s+"/"+p[1]+"/"+p[2]+"/"+vid+"?callback=utag.ut%5B%22writeva"+p[2]+"%22%5D&rnd="+t});}
u.do_enrichment=function(server,enrichment_polling,enrichment_polling_delay){if(typeof utag.ut.loader!="undefined"){for(var i=0;i<enrichment_polling;i++){setTimeout(function(){u.visitor_service_request((new Date).getTime(),server)},i*enrichment_polling_delay+1);}}}}
var json_string;var regExpReplace=new RegExp((u.visitor_id||"CouldNotFindAVisitorIdInThisProfileUpgradeUtagToGetVisitorIdSet"),"g");if(window.FormData){function postData(server_index,enrichment_polling,enrichment_polling_delay){if(server_index+1>u.server_list.length){return;}
var xhr=new XMLHttpRequest();var server=u.server_list[server_index];var formData=new FormData();xhr.addEventListener('readystatechange',function(){if(xhr.readyState===3){try{u.region=xhr.getResponseHeader("X-Region")||u.region||"";}catch(res_error){utag.DB(res_error);u.region=u.region||"";}
if(u.region)utag.loader.SC("utag_main",{"dc_region":u.region+";exp-session"});utag.DB("dc_region:"+u.region);}else if(xhr.readyState===4){postData(server_index+1,enrichment_polling,enrichment_polling_delay);if(u.make_enrichment_request&&u.enrichment_enabled[server])u.do_enrichment(server,enrichment_polling,enrichment_polling_delay);}});xhr.open('post',u.server_list[server_index],true);xhr.withCredentials=true;json_string=JSON.stringify(u.data);if(u.profile_specific_vid==1){json_string=json_string.replace(regExpReplace,u.visitor_id+u.get_account_profile(server)[2]);}
formData.append("data",json_string);xhr.send(formData);}
postData(0,u.enrichment_polling,u.enrichment_polling_delay);}else{for(var i=0;i<u.server_list.length;i++){(function(i,enrichment_polling,enrichment_polling_delay){var server=u.server_list[i];setTimeout(function(){json_string=JSON.stringify(u.data);if(u.profile_specific_vid==1){json_string=json_string.replace(regExpReplace,u.visitor_id+u.get_account_profile(server)[2]);}
var img=new Image();img.src=server+'?data='+encodeURIComponent(json_string);if(u.make_enrichment_request&&u.enrichment_enabled[server])u.do_enrichment(server,enrichment_polling,enrichment_polling_delay);},i*700);}(i,u.enrichment_polling,u.enrichment_polling_delay))}}}}
try{utag.o[loader].loader.LOAD(id)}catch(e){utag.loader.LOAD(id)}})('103','ti.main');}catch(e){utag.DB(e)}