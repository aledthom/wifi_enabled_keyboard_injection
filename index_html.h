const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 WiFi keyboard injector</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #2c257a;}
.box{ padding: 40px; margin:40px; background-color: #191919; color: white; text-align: center; border-radius: 24px; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);}
h1{ text-transform: uppercase; font-weight: 500;}
textarea {  width: calc(100vw - 160px);  padding:6px; background-color: #191919;  color: white;  border: 5px solid #22a6b3; border-radius: 24px;  font-family: monospace;  font-size: 16px; overflow: auto; overflow-x: auto; white-space: pre; text-align:left; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);scrollbar-width: thin; scrollbar-color: #22a6b3 #191919;}
textarea:focus {  border-color: #00e3c5;}
input:focus{ border-color:#22a6b3 ;}
input[type='submit'],input[type='button']{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #22a6b3; padding: 14px 10px; width: 140px; outline: none; border-radius: 24px; color: white; transition: 0.3s; cursor: pointer;}
input[type='submit']:hover,input[type='button']:hover{ background-color: #22a6b3;}
.row {display: flex; justify-content: center;  align-items: center; gap: 16px; margin-bottom: 20px;}
select {  background-color: #191919; color: white; border: 2px solid #22a6b3; border-radius: 24px; padding: 10px 14px; font-size: 16px; box-shadow: 0px 1px 12px rgba(0,227,197,0.3); width:50vw;}
</style>
</head>
<body>
<div class="box">
<h1>ESP32 WiFi keyboard injector</h1>
<form action="/file" class="row">
  <select name="script_select">
    %SELECT_LIST%
  </select>  
  <input type="submit" name="action" value="Load">
  <input type="button" popovertarget="mypopover" popovertargetaction="show" value="Delete">
  <div id=mypopover popover class="box">
    Delete: Are you sure?
    <div class=row >
      <input type="submit" name="action" value="Delete">
      <input type="button" popovertarget="mypopover" popovertargetaction="hide" value="No">
    </div>
  </div>
</form>
<div class="row">
  <h3>Loaded Script: %SCRIPT_FILENAME%</h3>
  <a href="/run"><input type="submit" value="Run"></a>
</div>
<div>
  <textarea id="display_textarea" name="display_textarea" rows="10" readonly >
  %SCRIPT_TXT%
  </textarea>
</div>
<form class="row" action="/edit">
  <input type="submit" name="action" value="New Script">
  <input type="submit" name="action" value="Edit">
</form>
</body></html>
)rawliteral";
