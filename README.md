# Humidifier Project
Humidifier Project (PT. Mursmedic Jaya Mandiri)

## Service API (esp32)
### End-point untuk device register dan publish data.
* Cheatsheet Table

Fungsi        | Url API            | Method
------------- | -------------      | -------------
Register      | /service/register/ | GET
Publish Data  | /service/publish/  | POST

* __Register API__
  
  Register API digunakan untuk mendaftarkan device (humidifier) ke server.
  Setelah device terdaftar, maka device bisa duhubungkan atau _linking_ ke akun RS.
  
  Request Body (JSON)
  
  ```
    {
      deviceID : <kode-mesin>
    }
  ```
  Body json diatas berupa kode mesin humidifier untuk mendaftarkan device ke server.
  Kode mesin digunakan untuk tanda pengenal. _(unique)_
  

* Publish data API
  ```
    {
    'deviceID': <kode-mesin>,
    'suhu_chamber_a': <string:chamber-a>,
    'suhu_chamber_b': <string:chamber-b>,
    'suhu_output_pasien': <string:output-pasien>,
    'suhu_heater_plate': <string:heater-plate>,
    'arus_heater_plate': <string:arus-heater-plate>,
    'arus_heater_wire': <string:arus-heater-wire>,
    'code' : <string:chamber-b>,
    }
 
  ```
  Body json diatas berupa kode mesin humidifier untuk mendaftarkan device ke server.
  Kode mesin digunakan untuk tanda pengenal. _(unique)_  
  
dari page
