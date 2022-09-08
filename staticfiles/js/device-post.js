
let incomingDeviceData = {};
var all_websocket = []

// Konek ke setiap device yang ada (websocket)
for(let i=0;i<device_timelapse.length; i++){
    all_websocket[i] = new WebSocket('ws://' + window.location.host + '/streaming/' + device_timelapse[i]['id'] + '/');
    all_websocket[i].onopen = function (e) {console.log("connect " + device_timelapse[i]['id']);}
    all_websocket[i].onclose = function (e) {console.log("connect " + device_timelapse[i]['id']);}

    // Websocket menerima message
    all_websocket[i].onmessage = function (e) {
        incomingDeviceData = (JSON.parse(e.data))['data'];
    
        // update data on webpage meneyesuaikan element css nya
        for(let i=0;i<device_timelapse.length; i++){
            let device = device_timelapse[i];


            if (device['id']==incomingDeviceData['deviceID']){
                changeTemp(incomingDeviceData['deviceID']);
                device['timelive'] = 10;

            }

        }
    }   
}


function changeTemp(deviceID){
    document.getElementById('temp-chamber-'+deviceID).textContent = incomingDeviceData['suhu_chamber_a'];
}

function disconnectedDevice(deviceID){
    document.getElementById('card-'+deviceID).classList.remove('active');
    document.getElementById('card-'+deviceID).classList.add('deactivate');

    report(deviceID);
}

function connectedDevice(deviceID){
    document.getElementById('card-'+deviceID).classList.remove('deactivate');
    document.getElementById('card-'+deviceID).classList.add('active');
}




// Fungsi timer untuk mengganti warga background card jika koneksi terputus
var countdownTime = setInterval(function(){
        for(let i=0;i<device_timelapse.length; i++){
          let device = device_timelapse[i];

          // timelive = 0
          if (device['timelive'] == 0){

            if (device['status']=='True'){     
                disconnectedDevice(device['id']);
                device['status'] = 'False';
            }

          }

          // timelive > 0
          else{

            if (device['status']=='False'){    
                connectedDevice(device['id'])
                device['status'] = 'True';
            }
                device['timelive'] -= 1;

          }
        }
  }, 1000);

  console.log(device_timelapse.length);
