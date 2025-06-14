const char edit_html[] PROGMEM = R"rawliteral(
"<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Edit Script</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #2c257a;}
.box{ padding: 40px; margin:40px; background-color: #191919; color: white; text-align: center; border-radius: 24px; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);}
h1{ text-transform: uppercase; font-weight: 500;}
textarea {  width: calc(100vw - 160px);  padding:6px; background-color: #191919;  color: white;  border: 5px solid #22a6b3; border-radius: 24px;  font-family: monospace;  font-size: 16px; overflow: auto; overflow-x: auto; white-space: pre; text-align:left; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);scrollbar-width: thin; scrollbar-color: #22a6b3 #191919;}
textarea:focus {  border-color: #00e3c5;}
.row {display: flex; justify-content: center;  align-items: center; gap: 16px; margin-bottom: 20px;}
input:focus{ border-color:#22a6b3 ;}
input[type='submit'],input[type='button']{ border: 0; display: block; background: none; margin: 20px auto; text-align: center; border: 2px solid #22a6b3; padding: 14px 10px; width: 140px; outline: none; border-radius: 24px; color: white; transition: 0.3s; cursor: pointer;}
input[type='submit']:hover,input[type='button']:hover{ background-color: #22a6b3;}
</style>
</head>
<body>
<form form action="/file" class="box">
<h3>Edit File: %SCRIPT_FILENAME%</h3>
<div>
<textarea id="script_data" name="script_data" rows="20" cols="80">
%SCRIPT_TXT%
</textarea>
</div>

<div id=mypopover popover class="box">
<div class=row>
Filename to save:
<input id="script_filename" name="script_filename" value="%SCRIPT_FILENAME%">
</div>
<div class=row >
<input type="submit" name="action" value="Save">  
<input type="button" popovertarget="mypopover" popovertargetaction="hide" value="Back">
</div>
</div>

<div class=row >
<input type="button" popovertarget="mypopover" value="Save">
<input type="submit" name="action" value="Cancel">
</div>
</form>

</body></html>"
)rawliteral";
