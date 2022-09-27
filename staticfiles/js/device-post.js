// 
//  Javascript ini mengatur seluruh action yg di front-end '/monitoring/'
//  Setiap device akan memulai koneksi websocket dalam looping sebanyak jumlah device (device_timelapse)
//  device_timelapse merupakan Array yang beisi total device yang dimiliki, element didalamnya berbentuk dict
//  yang mengandung id, timelive, status, code.  ex: [ {}, {}, .., .. ]
// 
//  Pada saat page dimuat, semua timelive defaultnya adalah 1, ini untuk mentrigger semuanya menjadi merah (disconnected)
//  jika Websocket tadi terkoneksi dengan baik, maka setiap data yg diterima dari ws akan mengganti timelive device tsb
//  menjadi 10, dan akan berkurang 1 setiap detiknya. Maka, jika dalam 10 hitungan tidak ada data masuk dari Websocket
//  timelive akan menjadi 0 dan device dinyatakan disconnected.


let incomingDeviceData = {};
var all_websocket = []

// Konek ke setiap device yang ada (websocket)
for(let i=0;i<device_timelapse.length; i++){
    all_websocket[i] = new WebSocket('ws://' + window.location.host + '/streaming/' + device_timelapse[i]['id'] + '/');
    all_websocket[i].onopen = function (e) {
        console.log("WS connected " + device_timelapse[i]['id']);
    }
    all_websocket[i].onclose = function (e) {
        console.log("WS connection closed from " + device_timelapse[i]['id']);
    }

    // Websocket menerima message
    all_websocket[i].onmessage = function (e) {
        incomingDeviceData = (JSON.parse(e.data))['data'];
        console.log(incomingDeviceData);
        console.log('===');
        // update data on webpage meneyesuaikan element css nya
        for(let i=0;i<device_timelapse.length; i++){
            let device = device_timelapse[i];


            if (device['id']==incomingDeviceData['deviceID']){
                changeTemp(incomingDeviceData['deviceID']);
                device['timelive'] = 6;

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

    //trace device on array
    device_timelapse.forEach(device => {
        //  jika device ditemukan maka ubah statusnya ke false lalu kirim sinyal disconnect
        if (device['id'] == deviceID){
            device['status'] = 'False';
            report(deviceID);
        }
    });
    
}

function connectedDevice(deviceID){
    document.getElementById('card-'+deviceID).classList.remove('deactivate');
    document.getElementById('card-'+deviceID).classList.add('active');

    console.log('change ' + deviceID + 'status True');
    //trace device on array
    device_timelapse.forEach(device => {
        //  jika device ditemukan maka ubah statusnya ke false lalu kirim sinyal disconnect
        if (device['id'] == deviceID){
            device['status'] = 'True';
        }
    });
}


// Fungsi timer untuk memberi sinyal kepada front-end jika koneksi http dari device terputus (setiap 1 detik)
var countdownTime = setInterval(function(){

        device_timelapse.forEach(device => {

            // jika timelive device variable sudah 0, artinya koneksi yg ditunggu selama 10 detik tidak datang
            // maka akan kirim sinyal disconnect ke server
            if (device['timelive'] == 0) {

                if (device['status'] == 'True'){
                    disconnectedDevice(device['id']);
                }

            }else{

                if (device['status'] == 'False'){
                    connectedDevice(device['id']);
                }

                //kurangi timelive device setiap 1 detiknya
                device['timelive'] -= 1;
            }
        });

  }, 1000);
