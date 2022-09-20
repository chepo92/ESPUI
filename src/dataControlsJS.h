const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPDATE_ACCEL=118;const UI_SEPARATOR=19;const UPDATE_SEPARATOR=119;const UI_TIME=20;const UPDATE_TIME=120;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_DARK:case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>(data.controls.length-1)){websock.send("uiok:"+(data.controls.length-1));}
break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>data.startindex+(data.controls.length-1)){websock.send("uiok:"+(data.startindex+(data.controls.length-1)));}
break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GAUGE:case UI_SEPARATOR:if(data.visible)addToHTML(data);break;case UI_BUTTON:if(data.visible){addToHTML(data);$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:if(data.visible){addToHTML(data);switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:if(data.visible){addToHTML(data);$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:if(data.visible){addToHTML(data);rangeSlider(!sliderContinuous);}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
break;case UI_MIN:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("min",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){if($('#sl'+data.parentControl).length){$('#sl'+data.parentControl).attr("max",data.value);}else if($('#text'+data.parentControl).length){$('#text'+data.parentControl).attr("maxlength",data.value);}else if($('#num'+data.parentControl).length){$('#num'+data.parentControl).attr("max",data.value);}}
break;case UI_STEP:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("step",data.value);}}
break;case UI_GRAPH:if(data.visible){addToHTML(data);graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_ACCEL:if(hasAccel)break;hasAccel=true;if(data.visible){addToHTML(data);requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:$("#sl"+data.id).attr("value",data.value)
slider_move($("#sl"+data.id).parent().parent(),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
if(data.hasOwnProperty('inputType')){$("#text"+data.id).attr("type",data.inputType);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_BUTTON:$("#btn"+data.id).val(data.value);$("#btn"+data.id).text(data.value);if(data.hasOwnProperty('elementStyle')){$("#btn"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;case UPDATE_TIME:var rv=new Date().toISOString();websock.send("time:"+rv+":"+data.id);break;default:console.error("Unknown type or event");break;}
if(data.type>=UI_TITEL&&data.type<UPDATE_OFFSET){processEnabled(data);}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.hasOwnProperty('visible')){if(data['visible'])
$("#id"+data.id).show();else
$("#id"+data.id).hide();}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}
processEnabled(data);}
$(".range-slider__range").each(function(){$(this)[0].value=$(this).attr("value");$(this).next().html($(this).attr("value"));});};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);$(".range-slider__range").each(function(){$(this).attr("value",$(this)[0].value);});}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){if($("#id"+number+" nav").hasClass("disabled")){return;}
switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if(!$("#sl"+number).hasClass("checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var slidercb=function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));};range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){$(this).next().html(this.value);if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:slidercb});}else{$(this).on({change:slidercb});}
$(this).attr("callbackSet","true");}});};var addToHTML=function(data){panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"' ":"";panelwide=data.hasOwnProperty('wide')?"wide":"";if(!data.hasOwnProperty('parentControl')||$("#tab"+data.parentControl).length>0){var parent=data.hasOwnProperty('parentControl')?$("#tab"+data.parentControl):$("#row");var html="";switch(data.type){case UI_LABEL:case UI_BUTTON:case UI_SWITCHER:case UI_CPAD:case UI_PAD:case UI_SLIDER:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GRAPH:case UI_GAUGE:case UI_ACCEL:html="<div id='id"+data.id+"' "+panelStyle+" class='two columns "+panelwide+" card tcenter "+
colorClass(data.color)+"'><h5>"+data.label+"</h5><hr/>"+
elementHTML(data)+
"</div>";break;case UI_SEPARATOR:html="<div id='id"+data.id+"' "+panelStyle+" class='sectionbreak columns'>"+
"<h5>"+data.label+"</h5><hr/></div>";break;case UI_TIME:break;}
parent.append(html);}else{var parent=$("#id"+data.parentControl);parent.append(elementHTML(data));}}
var elementHTML=function(data){var id=data.id
var elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"' ":"";var inputType=data.hasOwnProperty('inputType')?" type='"+data.inputType+"' ":"";switch(data.type){case UI_LABEL:return"<span id='l"+id+"' "+elementStyle+
" class='label label-wrap'>"+data.value+"</span>";case UI_BUTTON:return"<button id='btn"+id+"' "+elementStyle+
" onmousedown='buttonclick("+id+", true)'"+
" onmouseup='buttonclick("+id+", false)'>"+
data.value+"</button>";case UI_SWITCHER:return"<label id='sl"+id+"' "+elementStyle+
" class='switch "+(data.value=="1"?"checked":"")+
(data.hasOwnProperty('vertical')?" vert-switcher ":"")+
"'>"+
"<div class='in'>"+
"<input type='checkbox' id='s"+id+"' onClick='switcher("+id+",null)' "+
(data.value=="1"?"checked":"")+"/></div></label>";case UI_CPAD:case UI_PAD:return"<nav class='control'><ul>"+
"<li><a onmousedown='padclick(UP, "+id+", true)' "+
"onmouseup='padclick(UP, "+id+", false)' id='pf"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+id+", true)' "+
"onmouseup='padclick(RIGHT, "+id+", false)' id='pr"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+id+", true)' "+
"onmouseup='padclick(LEFT, "+id+", false)' id='pl"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+id+", true)' "+
"onmouseup='padclick(DOWN, "+id+", false)' id='pb"+id+"'>&#9650;</a></li>"+
"</ul>"+
(data.type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+id+", true)' "+
"onmouseup='padclick(CENTER, "+id+", false)' id='pc"+id+"'>OK</a>":"")+
"</nav>";case UI_SLIDER:return"<div class='range-slider "+
(data.hasOwnProperty('vertical')?" vert-slider ":"")+
"'>"+
"<input id='sl"+id+"' type='range' min='0' max='100' value='"+data.value+"' "+
elementStyle+" class='range-slider__range'><span class='range-slider__value'>"+
data.value+"</span></div>";case UI_NUMBER:return"<input style='color:black; "+data.elementStyle+"' id='num"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_TEXT_INPUT:return"<input "+inputType+"style='color:black; "+data.elementStyle+"' id='text"+id+
"' value='"+data.value+"' onchange='textchange("+id+")' />";case UI_SELECT:return"<select style='color:black; "+data.elementStyle+"' id='select"+id+
"' onchange='selectchange("+id+")' />";case UI_GRAPH:return"<figure id='graph"+id+"'><figcaption>"+data.label+"</figcaption></figure>";case UI_GAUGE:return"WILL BE A GAUGE <input style='color:black;' id='gauge"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_ACCEL:return"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+id+
"' ><div class='ball"+id+"'></div><pre class='accelerometeroutput"+id+"'></pre>";default:return"";}}
var processEnabled=function(data){switch(data.type){case UI_SWITCHER:case UPDATE_SWITCHER:if(data.enabled){$("#sl"+data.id).removeClass('disabled');$("#s"+data.id).prop("disabled",false);}else{$("#sl"+data.id).addClass('disabled');$("#s"+data.id).prop("disabled",true);}
break;case UI_SLIDER:case UPDATE_SLIDER:$("#sl"+data.id).prop("disabled",!data.enabled);break;case UI_NUMBER:case UPDATE_NUMBER:$("#num"+data.id).prop("disabled",!data.enabled);break;case UI_TEXT_INPUT:case UPDATE_TEXT_INPUT:$("#text"+data.id).prop("disabled",!data.enabled);break;case UI_SELECT:case UPDATE_SELECT:$("#select"+data.id).prop("disabled",!data.enabled);break;case UI_BUTTON:case UPDATE_BUTTON:$("#btn"+data.id).prop("disabled",!data.enabled);break;case UI_PAD:case UI_CPAD:case UPDATE_PAD:case UPDATE_CPAD:if(data.enabled){$("#id"+data.id+" nav").removeClass('disabled');}else{$("#id"+data.id+" nav").addClass('disabled');}
break;}}
)=====";

const uint8_t JS_CONTROLS_GZIP[4219] PROGMEM = { 31,139,8,0,42,225,211,98,2,255,197,27,105,119,218,72,242,187,127,133,172,228,5,177,198,28,185,38,3,150,243,8,38,9,59,190,214,198,147,121,155,201,250,9,104,140,158,133,164,145,132,29,143,227,255,190,213,213,135,186,117,0,198,51,187,95,98,84,93,93,93,93,85,221,117,117,198,129,31,39,198,197,224,114,112,60,24,14,186,135,151,159,46,6,246,203,102,179,51,22,3,103,253,195,147,238,1,192,90,2,118,122,208,29,246,47,79,62,126,60,239,15,237,150,138,219,255,109,216,63,62,96,52,90,10,124,56,24,246,15,109,5,112,10,20,51,244,16,148,46,50,184,236,81,200,75,29,169,199,176,94,166,88,31,46,134,195,147,99,251,149,142,199,161,173,230,171,20,243,176,251,1,152,120,173,35,50,96,171,249,58,197,59,255,50,24,246,62,247,207,236,55,58,170,132,183,154,111,20,236,195,193,1,192,222,102,112,25,180,213,124,155,98,30,95,28,125,0,216,79,58,38,135,182,154,63,41,226,2,57,130,66,78,47,134,246,59,29,91,25,105,53,223,165,51,62,157,117,79,63,219,63,115,64,247,224,128,65,46,79,79,6,199,20,149,15,244,14,251,221,51,142,220,106,254,172,172,216,253,96,183,50,10,97,48,69,33,231,253,195,126,15,168,101,116,34,192,45,69,43,39,167,195,1,149,127,70,45,2,220,82,212,114,52,0,64,70,43,8,107,41,58,57,234,254,102,183,50,250,64,88,75,85,197,176,127,106,183,178,154,64,96,75,209,195,167,238,197,167,190,221,202,232,129,67,91,138,26,186,189,30,181,141,140,10,56,180,245,78,21,204,105,247,172,59,60,1,53,254,156,149,141,28,105,169,226,30,28,245,225,68,101,228,77,129,45,5,42,15,204,193,201,151,99,121,92,14,251,31,135,242,88,156,13,62,125,30,74,227,239,245,143,135,96,75,66,110,189,203,225,197,217,191,46,78,6,231,125,73,170,119,217,63,234,159,117,15,211,227,215,187,60,237,195,172,179,193,175,48,245,165,4,126,233,15,187,231,167,159,187,135,10,249,203,243,139,227,143,135,39,95,180,53,122,221,179,179,147,161,60,44,189,203,238,225,224,223,221,51,208,224,91,9,58,232,158,253,34,13,191,119,121,124,114,12,219,127,243,166,115,227,68,198,85,228,132,179,3,39,113,108,159,220,26,221,40,114,238,172,42,142,204,156,184,59,30,19,207,158,58,94,76,16,20,123,238,132,68,189,192,79,92,127,17,44,98,62,52,93,248,227,196,13,124,99,28,120,65,212,243,156,56,182,240,231,96,82,189,231,63,236,227,197,124,68,34,9,239,196,183,110,50,158,41,120,78,76,84,153,181,35,146,44,34,223,132,127,254,88,4,110,76,204,14,71,225,34,20,8,100,78,34,199,155,200,225,84,158,2,35,36,9,137,34,247,134,68,18,41,149,175,64,186,37,137,19,135,51,199,75,36,146,148,183,192,137,23,254,212,11,110,21,58,76,252,98,124,236,68,81,144,206,23,170,16,195,142,231,254,233,68,174,47,17,168,98,218,252,55,213,138,64,156,56,209,181,217,153,144,169,179,240,18,1,52,59,15,15,91,84,11,183,100,20,7,227,235,142,242,27,84,226,147,113,66,38,89,141,68,228,143,5,137,147,147,200,37,126,226,80,208,41,137,230,110,28,195,47,171,122,255,176,37,49,99,231,134,124,18,198,0,67,94,48,118,188,243,36,136,156,43,82,143,73,50,72,200,220,50,73,28,46,92,180,153,216,172,253,243,252,228,184,30,39,176,163,43,119,122,103,73,83,170,86,59,10,225,8,214,15,34,133,182,11,218,70,107,130,21,39,104,122,218,90,87,133,107,165,196,59,238,212,146,83,183,109,127,225,121,213,251,148,22,50,21,58,81,76,82,172,106,135,201,48,93,242,171,59,249,6,76,50,240,215,111,89,126,157,40,1,17,60,183,38,193,120,49,7,201,85,235,206,100,98,153,255,48,171,245,96,58,133,3,242,220,50,159,69,193,45,124,207,146,185,103,153,102,181,195,85,81,31,123,1,172,13,6,206,168,168,164,225,4,158,131,22,22,113,31,236,36,130,21,74,212,71,169,75,92,88,35,34,243,224,134,176,131,101,226,137,217,189,138,8,241,205,106,14,21,248,212,240,34,50,41,192,98,76,35,23,70,195,56,14,12,206,1,101,242,197,179,119,111,95,189,233,20,204,74,183,174,65,125,235,126,236,185,227,235,54,151,92,237,65,219,244,204,241,39,30,249,213,141,221,145,235,185,201,93,15,0,87,32,160,123,208,227,118,86,0,47,94,108,11,153,215,103,238,100,66,252,234,189,84,8,61,0,169,189,114,37,73,116,216,121,255,6,126,28,186,113,66,124,184,106,204,27,185,230,24,215,52,107,197,188,212,80,234,104,88,183,174,63,9,110,235,212,32,233,42,245,48,136,146,109,219,52,127,252,40,30,121,215,44,27,121,253,250,149,212,47,222,173,95,200,232,28,126,147,196,50,111,227,118,163,97,238,100,39,206,130,56,241,157,57,217,49,219,249,65,74,117,199,108,220,130,200,59,15,4,248,221,152,56,167,177,37,12,54,240,131,144,248,182,144,172,69,110,18,122,109,251,113,224,17,152,127,5,36,25,166,65,241,214,176,185,50,219,76,200,119,224,79,234,58,61,51,169,253,39,209,130,116,30,58,41,107,120,154,214,225,13,17,129,100,246,140,105,212,8,133,45,161,70,1,69,20,152,63,164,182,131,38,182,138,2,69,159,100,46,35,24,168,79,240,46,162,163,196,150,118,59,10,38,119,8,27,195,23,137,192,216,132,111,164,232,245,228,46,36,220,59,234,121,66,187,224,14,162,160,196,25,197,190,115,83,4,6,70,225,104,36,234,16,216,60,46,147,117,237,112,165,102,157,125,33,30,216,17,194,41,233,40,240,226,250,52,136,250,14,48,79,60,66,183,103,239,227,93,15,158,236,59,19,221,61,69,111,103,92,7,71,174,214,30,58,138,152,45,57,11,116,144,178,154,4,137,227,137,245,246,45,125,121,143,248,87,201,108,183,85,149,135,15,156,151,15,119,247,194,13,174,225,88,149,162,195,70,70,17,113,174,59,66,212,105,54,213,254,255,238,144,201,157,94,119,112,164,201,247,157,141,118,188,206,252,188,12,88,246,217,206,222,37,17,241,2,103,2,39,67,199,198,60,179,45,13,59,113,19,143,48,171,241,156,17,241,208,12,231,142,235,127,38,14,216,144,176,194,20,33,75,15,179,195,182,248,98,169,154,252,76,115,49,9,98,169,144,252,196,156,66,25,228,185,64,91,72,25,221,131,71,170,112,125,13,131,207,195,163,67,139,157,79,157,9,150,203,230,38,221,103,103,209,205,141,18,223,220,65,52,136,113,208,49,38,193,98,60,67,217,183,211,59,163,122,79,234,97,68,168,218,15,88,140,71,69,185,72,18,122,221,129,35,181,56,137,26,189,13,65,41,53,164,2,58,221,132,6,247,109,15,204,43,235,123,19,57,245,234,221,177,27,9,156,170,32,203,208,29,15,25,204,144,165,101,2,41,119,250,123,45,225,133,211,141,101,23,58,19,182,233,139,211,218,166,178,43,162,161,202,14,89,244,158,206,34,77,32,159,206,164,70,37,199,102,244,116,54,49,181,125,58,159,58,153,28,163,163,167,51,74,179,243,167,243,169,81,201,177,57,126,58,155,172,56,240,116,70,51,116,150,158,110,172,130,173,62,124,17,141,129,207,209,175,91,219,185,56,32,71,118,216,253,144,167,169,7,30,78,24,162,7,218,243,220,253,61,199,8,252,57,80,34,139,208,174,0,14,219,135,148,232,142,89,173,24,179,136,76,237,10,165,160,192,43,251,252,3,175,153,29,115,175,225,236,239,53,128,100,97,68,35,215,156,184,55,134,59,193,165,52,98,123,13,24,161,115,233,60,27,8,212,117,2,240,53,34,147,30,251,182,32,255,13,194,54,85,211,67,21,227,54,203,116,66,151,173,236,0,246,212,245,32,88,179,164,218,104,154,130,41,230,115,43,153,185,49,240,147,36,144,130,208,141,153,85,219,182,33,18,166,251,222,77,130,93,118,151,66,58,143,198,100,34,220,172,169,6,64,227,9,96,199,14,163,96,30,66,200,60,116,70,70,18,24,108,34,253,101,249,88,77,49,32,125,3,155,124,207,226,184,109,186,159,58,67,130,25,22,124,66,76,224,120,36,162,36,102,78,66,105,26,147,128,196,134,31,36,6,249,14,137,146,209,254,253,119,76,6,10,108,174,192,164,88,5,81,170,31,34,91,152,209,99,145,4,99,155,129,168,120,159,197,16,243,140,19,174,3,29,181,195,62,83,165,5,33,38,117,84,111,236,167,185,179,37,116,183,101,86,12,52,1,187,34,160,204,34,232,128,128,176,197,8,197,222,23,48,12,42,0,176,215,96,52,247,205,252,142,142,6,165,219,1,240,115,171,242,44,246,42,69,91,224,113,19,53,253,114,28,102,4,115,215,55,117,135,73,243,55,131,211,7,85,174,94,160,20,169,108,133,220,62,187,191,253,253,251,116,190,47,217,39,205,254,86,175,80,142,37,215,96,232,127,183,68,115,123,201,93,175,195,254,233,154,39,193,157,20,157,130,29,211,112,253,112,145,176,211,203,79,68,236,254,73,44,56,182,226,128,32,51,113,66,194,21,220,96,87,97,245,93,47,203,104,95,249,233,250,102,231,234,115,194,213,117,128,3,240,5,56,112,126,115,101,229,231,214,76,132,165,222,81,63,94,153,70,72,27,175,181,216,62,114,146,89,61,10,22,112,240,105,233,2,40,194,142,105,217,111,232,206,225,87,163,213,108,54,139,56,173,135,139,120,102,221,127,111,39,113,237,174,157,74,3,238,169,76,241,114,19,206,21,190,149,62,77,187,64,96,95,191,253,197,235,137,102,7,85,159,168,186,87,217,184,44,194,99,97,100,181,43,95,90,238,205,92,126,74,11,14,107,9,106,112,155,102,102,220,224,196,210,192,208,201,173,127,26,5,33,184,149,59,171,194,243,218,243,228,206,35,149,42,11,4,84,66,194,128,97,152,91,176,58,163,152,37,153,150,44,201,58,192,163,54,205,247,205,118,235,113,172,197,79,230,141,5,85,37,180,144,53,237,164,110,177,128,234,146,22,112,173,220,44,118,202,173,244,135,50,181,102,194,65,48,149,218,228,255,110,147,60,213,166,180,224,154,84,136,1,99,27,91,133,78,105,19,182,148,148,31,227,63,112,21,127,17,111,25,82,171,153,43,35,142,23,250,240,46,92,78,153,214,244,56,97,57,161,196,220,88,73,35,23,78,61,117,195,57,98,155,232,131,151,70,242,149,143,44,103,121,12,44,5,111,202,188,78,106,19,206,211,2,69,250,176,161,157,71,99,21,36,186,228,149,179,184,34,127,145,236,179,180,54,217,0,243,23,5,71,100,112,212,71,63,27,221,216,138,115,77,130,193,249,201,57,22,35,173,180,83,197,202,132,9,120,221,182,185,19,221,96,219,33,227,159,68,15,82,148,184,177,128,110,153,23,254,181,31,220,250,6,53,100,154,132,96,230,96,138,73,233,241,160,227,251,182,40,13,190,120,33,129,123,218,51,22,8,119,162,96,76,226,184,239,59,224,215,38,220,155,229,232,168,115,52,98,90,97,156,133,95,162,58,171,197,95,116,99,101,170,10,29,159,120,154,162,212,105,5,90,74,39,44,187,17,184,175,166,52,57,202,87,9,251,86,221,202,173,19,207,130,91,208,17,141,103,243,131,51,240,37,86,86,48,182,173,249,166,234,61,223,186,222,54,100,126,104,87,182,211,121,51,127,151,183,207,197,103,218,43,23,144,180,49,46,32,178,13,46,0,172,239,45,190,100,155,155,238,130,136,222,156,206,134,185,163,188,21,224,245,104,248,174,138,222,86,225,22,82,222,5,211,10,183,42,155,146,63,206,216,50,142,74,249,216,42,177,73,90,57,192,146,201,46,119,237,151,248,101,86,235,132,118,5,148,138,0,47,5,124,109,126,227,49,139,94,27,96,193,2,189,29,25,212,167,119,34,15,189,10,49,89,74,174,180,179,230,192,158,115,69,108,165,163,160,118,94,25,127,172,247,201,43,6,236,108,0,61,91,4,10,28,142,55,90,246,106,136,61,92,25,174,5,248,139,215,3,199,238,60,90,8,122,128,148,149,12,47,54,72,206,217,50,75,56,199,96,98,25,235,126,25,231,202,42,212,9,45,89,131,185,238,101,139,36,235,44,34,42,94,133,75,208,26,213,210,21,156,209,26,107,48,111,190,76,209,220,221,47,85,246,26,11,169,13,6,54,92,115,227,9,120,2,188,220,248,79,157,236,136,194,20,106,152,168,103,80,22,97,241,114,178,234,73,175,185,90,126,65,113,63,178,17,200,167,121,223,211,137,249,141,49,113,99,60,188,112,118,120,125,14,200,243,238,170,210,88,101,69,213,118,217,22,194,241,234,61,132,99,109,19,154,103,46,39,60,93,131,240,180,140,48,173,89,151,147,94,67,238,225,168,140,52,109,47,148,147,246,214,32,237,149,145,198,142,64,57,237,104,13,218,81,1,109,237,105,136,72,27,185,197,196,144,10,19,52,24,252,101,243,55,67,180,100,154,189,5,83,211,1,2,227,107,102,57,153,115,226,192,42,55,68,191,12,53,34,202,123,8,65,68,127,178,33,40,185,254,74,90,250,211,159,148,156,44,120,241,45,181,210,212,111,25,27,89,28,240,113,97,58,46,218,18,25,218,205,60,237,18,182,86,144,23,221,10,246,142,77,233,59,164,15,42,220,248,192,141,199,112,86,121,25,28,145,236,82,119,163,60,75,28,143,108,197,237,104,190,79,247,65,112,99,208,13,132,158,51,38,86,227,63,191,31,236,52,174,106,38,186,87,214,11,193,30,15,38,102,237,2,71,166,186,105,10,225,30,236,225,129,119,82,74,61,96,201,196,14,94,100,42,131,99,199,243,70,206,248,250,156,64,76,189,109,155,84,43,38,179,87,85,60,98,82,202,173,16,196,131,176,55,21,133,201,66,195,217,42,95,183,198,87,237,60,60,136,7,48,178,216,148,202,25,99,162,251,52,16,182,87,70,214,239,77,3,99,104,90,203,207,68,209,59,180,156,223,54,205,14,130,110,129,205,98,114,116,132,18,162,127,17,159,10,166,100,97,165,220,90,169,254,248,33,125,110,121,117,120,191,169,149,111,215,161,251,126,25,217,244,153,14,123,69,4,218,95,254,196,71,127,245,192,115,236,92,199,190,180,215,158,233,254,109,254,122,2,203,159,197,111,41,88,246,201,182,34,251,109,74,162,130,154,220,81,53,107,140,233,85,97,87,146,219,128,190,18,94,204,253,88,96,80,53,82,4,39,154,24,9,123,3,69,187,58,197,113,57,54,242,102,111,68,103,144,117,120,204,189,6,128,246,102,81,131,182,126,120,132,159,86,69,177,3,132,189,191,76,209,53,125,18,178,201,86,98,246,94,18,73,138,61,209,150,37,172,182,132,193,98,78,48,109,23,201,179,222,27,163,172,137,227,188,86,91,33,219,92,203,201,67,94,192,202,72,246,72,211,97,144,4,23,130,138,189,228,156,235,197,142,220,73,87,135,229,89,199,133,68,13,204,94,85,83,3,154,152,241,10,146,114,72,210,91,117,174,248,99,234,189,24,84,138,186,6,111,37,180,172,49,184,37,21,141,58,52,240,223,221,219,200,9,243,125,105,74,108,223,204,62,25,18,75,177,160,25,23,195,202,85,217,114,188,77,78,3,32,192,84,34,109,54,165,102,160,119,166,61,80,83,109,169,23,97,50,71,91,217,215,251,165,192,41,67,86,120,149,23,138,224,150,237,150,50,27,175,22,13,239,72,139,71,102,162,50,223,50,223,75,167,15,74,129,19,88,82,27,129,63,46,120,29,84,44,253,216,21,161,155,193,231,153,252,68,209,147,201,215,116,125,14,67,229,115,123,192,213,70,193,247,10,227,92,48,30,248,61,42,23,193,40,125,20,204,36,132,17,32,118,143,87,112,110,138,35,187,215,64,201,40,162,203,93,189,66,132,144,133,8,102,249,43,59,184,180,22,30,227,90,123,18,193,116,173,62,61,50,116,101,83,14,77,69,217,133,168,92,219,184,117,250,138,138,189,119,120,241,236,231,183,111,154,157,244,213,196,170,213,217,91,157,117,25,200,96,107,60,68,27,243,128,239,154,214,101,65,71,214,56,240,54,230,0,95,2,173,203,129,142,172,113,48,90,202,65,131,153,131,86,196,99,22,245,222,4,214,82,235,153,186,209,188,82,194,42,127,11,180,38,175,28,187,152,219,177,224,246,228,23,202,168,56,125,123,13,176,101,245,182,96,65,133,48,116,229,84,170,209,121,122,172,214,56,240,124,134,126,220,217,209,214,47,33,118,208,113,157,138,49,119,65,16,77,248,235,124,183,43,173,102,83,121,33,162,94,120,40,13,221,239,20,49,204,211,9,56,165,232,22,10,49,144,98,193,141,138,119,191,240,234,153,104,75,200,137,109,135,123,67,12,100,218,35,200,62,174,59,70,137,107,164,59,199,18,23,127,254,194,246,206,178,170,210,173,130,19,192,16,95,32,242,228,135,201,15,20,221,80,24,84,226,63,157,73,192,78,125,234,35,57,102,5,179,236,139,157,50,38,149,234,91,33,139,60,30,149,14,27,75,88,143,21,162,40,124,113,166,210,213,181,146,89,225,250,44,0,22,203,79,221,171,69,68,144,38,239,228,243,87,101,48,48,118,248,243,162,76,204,167,12,225,7,16,80,233,99,72,205,233,127,25,28,30,26,31,250,70,215,64,176,81,110,50,108,95,188,135,244,119,153,7,11,240,57,111,248,97,52,232,255,19,130,51,57,15,153,152,201,196,152,130,19,189,219,86,111,1,135,190,85,32,81,48,167,117,121,198,41,130,36,167,251,42,246,8,242,77,41,71,230,100,67,144,113,17,173,96,145,208,151,50,18,57,68,81,150,253,15,57,189,114,159,141,109,203,67,196,76,118,149,121,141,32,122,46,132,145,45,232,181,171,85,145,138,168,61,86,120,89,68,109,251,99,89,68,22,39,101,93,132,231,236,217,14,190,168,227,60,134,36,175,228,20,63,9,93,231,65,67,150,224,182,182,247,76,254,162,101,151,43,222,16,60,138,112,46,79,93,231,37,192,163,86,208,210,222,213,77,247,71,209,214,146,247,21,61,243,71,17,86,35,207,94,182,153,93,216,220,46,52,94,45,207,229,197,243,50,27,78,141,179,104,86,161,141,10,235,123,120,248,47,91,237,76,5,232,64,0,0 };
