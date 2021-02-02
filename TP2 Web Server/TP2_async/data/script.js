/*function callServer(url,cFunction)
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200) 
    {
      cFunction(this);
    }
  };
  xhttp.open("GET", url, 1);
  xhttp.send();
}
function handleResponse(xhttp)
  {
    document.getElementById("state_LED").innerHTML = "LED" + xhttp.responseText;
  }
function handleResponse2(xhttp)
  {
    document.getElementById("state_b").innerHTML =  xhttp.responseText;
  }

setInterval(function handleResponse3(xhttp)
{
  document.getElementById("capteur_t").innerHTML =  xhttp.responseText;
}, 2000);
setInterval(function handleResponse4(xhttp)
{
  document.getElementById("capteur_h").innerHTML =  xhttp.responseText;
}, 2000);*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function callServer(ID,url)
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById(ID).innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", url, true);
    xhttp.send();
}
function ON() {
    callServer("state_LED","LEDisON")
}
function OFF() {
    callServer("state_LED", "LEDisOFF")
}

setInterval(function getData()
{
    callServer("capteur_h","humidity")
    callServer("capteur_t","temperature")
    callServer("state_b","bp")
}, 2000);
setInterval(function getData()
{
    callServer("count","compteur")
}, 500);