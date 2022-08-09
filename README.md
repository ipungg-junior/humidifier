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
  
  Request Header
  
  ```
    X-Machine-Code : XXXXXXX
  ```
  Tambahkan header tambahan diatas berupa kode mesin humidifier untuk mendaftarkan device ke server.
  Kode mesin digunakan untuk tanda pengenal. _(unique)_
  

* Publish data API
