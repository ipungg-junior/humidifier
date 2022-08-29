# Humidifier Project
Humidifier Project (PT. Mursmedic Jaya Mandiri)

## Service API (for esp32 device)
### End-point untuk device session dan publish data.

  API ini digunakan pada sisi device (esp32). Ada 2 end-point yang digunakan yaitu Register Session dan Publish Data.

* __Cheatsheet Table__

Fungsi        		| Url API            		| Method
------------- 		| -------------      		| -------------
Register Session     	| /service/register-session/ 	| GET
Publish Data  		| /service/publish/  		| POST


* __Register Session API__
  
  Register session API digunakan untuk meminta sesi baru kepada server untuk memenuhi proses Publish Data.
  Nantinya server akan memberi 'SessionID' untuk dimasukan ke request body proses Publish data.
  Sesi baru akan dimulai saat device humidifier pertama kali dinyalakan dan siap publish data,
  jika device mati atau restart, maka device harus meminta sesi baru. 
  
  
  Request Body (JSON)
  
  ```
    {
      deviceID : <kode-unik-mesin>
    }
  ```
  
  

* __Publish Data API__

  Publish Data API digunakan untuk mengirim atau publish data yang telah di olah dari semua sensor device.
  Seluruh data sensor/program/algoritma yang diolah humidifier akan dikirimkan ke server melalui API ini
  dengan menyertakan _sessionID_ pada request body.
  

  Request Body (JSON)
  ```
    {
    'deviceID': <string:kode-unik-mesin>,
    'sessionID': <string:sessionID>,
    'suhu_chamber_a': <string:chamber-a>,
    'suhu_chamber_b': <string:chamber-b>,
    'suhu_output_pasien': <string:output-pasien>,
    'suhu_heater_plate': <string:heater-plate>,
    'arus_heater_plate': <string:arus-heater-plate>,
    'arus_heater_wire': <string:arus-heater-wire>,
    'code' : <string:code-alert>,
    }
 
  ```

