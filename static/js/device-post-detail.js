let incomingDeviceData = {};
var alert, alertCounter;
// websocket object
const socket = new WebSocket('ws://' + window.location.host + '/streaming/' + deviceID + '/');
// websocket connected
socket.onopen = function (e) {
    console.log("Connection Made");
}
// websocket disconnected
socket.onclose = function (e) {
    console.log('Connection Closed');
}
 
// websocket receive data
socket.onmessage = function (e) {
    incomingDeviceData = (JSON.parse(e.data))['data'];
    // update data on webpage
    changeChamberA();
    changeOutputPasien();
    changeHeaterPlate();
    changeHeaterPlateStatus();
    changeHeaterWireStatus();
}   



// Suhu Chamber (actually chamber A)
function changeChamberA() {

    selector('chamber-a-number').textContent = incomingDeviceData['suhu_chamber_a'];
    
    if (parseInt(incomingDeviceData['suhu_chamber_a']) > 32){
        selector('chamber-A').style.border = "0.45em solid rgb(236, 91, 91)";
        selector('alert-chamber-a').style.display = 'initial';
        selector('chamber-a-number').style.color = 'rgb(235, 234, 234)';
    }else{
        selector('chamber-A').style.border = "0.45em solid #0eaebd";
        selector('alert-chamber-a').style.display = 'none';
        selector('chamber-a-number').style.color = 'black';
    }

    selector('chamber-A').style.transform = "rotate(" + (360 * incomingDeviceData['suhu_chamber_a'])/100 + "deg)"
}


// Suhu Output Pasien
function changeOutputPasien(){
    selector('output-pasien-number').textContent = incomingDeviceData['suhu_output_pasien'];

    if (parseInt(incomingDeviceData['suhu_output_pasien']) > 32){
        selector('output-pasien').style.border = "0.45em solid rgb(236, 91, 91)";
        selector('alert-output-pasien').style.display = 'initial';
        selector('output-pasien-number').style.color = 'rgb(235, 234, 234)';
    }else{
        selector('output-pasien').style.border = "0.45em solid #0eaebd";
        selector('alert-output-pasien').style.display = 'none';
        selector('output-pasien-number').style.color = 'black';
    }
 
    selector('output-pasien').style.transform = "rotate(" + (360 * incomingDeviceData['suhu_output_pasien'])/100 + "deg)"
}

// Suhu Heater Plate
function changeHeaterPlate(){
    selector('heater-plate-number').textContent = incomingDeviceData['suhu_heater_plate']

    if (parseInt(incomingDeviceData['suhu_heater_plate']) > 32){
        selector('heater-plate').style.border = "0.45em solid rgb(236, 91, 91)";
        selector('alert-heater-plate').style.display = 'initial';
        selector('heater-plate-number').style.color = 'rgb(235, 234, 234)';
    }else{
        selector('heater-plate').style.border = "0.45em solid #0eaebd";
        selector('alert-heater-plate').style.display = 'none';
        selector('heater-plate-number').style.color = 'black';
    }

    selector('heater-plate').style.transform = "rotate(" + (360 * incomingDeviceData['suhu_heater_plate'])/100 + "deg)"
}



// Heater Plate Status
function changeHeaterPlateStatus(){
    selector('alert-heater-plate-status').style.display = 'initial'

    if (parseInt(incomingDeviceData['arus_heater_plate']) > 0){
        selector('heater-plate-text').textContent = 'ON';
        selector('heater-plate-text').style.color = 'white';
        selector('alert-heater-plate-status').style.backgroundColor = '#0eaebd'
    }else{
        selector('heater-plate-text').textContent = 'OFF';
        selector('heater-plate-text').style.color = 'white';
        selector('alert-heater-plate-status').style.backgroundColor = '#737979'
    }

}

// Heater Wire Status
function changeHeaterWireStatus(){
    selector('alert-heater-wire-status').style.display = 'initial'

    if (parseInt(incomingDeviceData['arus_heater_wire']) > 0){
        selector('heater-wire-text').textContent = 'ON';
        selector('heater-wire-text').style.color = 'white';
        selector('alert-heater-wire-status').style.backgroundColor = '#0eaebd'
    }else{
        selector('heater-wire-text').textContent = 'OFF';
        selector('heater-wire-text').style.color = 'white';
        selector('alert-heater-wire-status').style.backgroundColor = '#737979'
    }

}


function connectedDevice(deviceID){
    document.getElementById('card-'+deviceID).style.backgroundColor = 'rgb(209, 216, 63)'
}
function disconnectedDevice(deviceID){
    document.getElementById('card-'+deviceID).style.backgroundColor = 'rgb(199, 63, 22)'
    report(deviceID);
}


function selector(id){
    return document.getElementById(id);
}


// Fungsi timer untuk mengganti warga background card jika koneksi terputus
setInterval(function(){
    console.log('Disconnect timer count.');
}, 5000);

