var csrf_token = document.getElementsByName('csrfmiddlewaretoken')[0].value;
var inputDeviceID = document.getElementById("input-id");
var confirmDeviceID = document.getElementById("confirmation-input-id");
var btnSubmit = document.getElementById("submit");

btnSubmit.addEventListener('click', function (e) {
    if (inputDeviceID.value != confirmDeviceID.value){
        alert("ID yang dimasukan tidak cocok!");
    }else{
        sendReqRegistrar();
    }
});
  
function sendReqRegistrar(){

    fetch("/supervisor/device-registrar/", {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
        }, 
        body: JSON.stringify({
            'deviceID': inputDeviceID.value,
        })
      }).then(res => {
        // print resp just for development, delete on production
        console.log(res);
      });
}