# Humidifier Project
Humidifier Project (PT. Mursmedic Jaya Mandiri)

## Service API (esp32)
#### End-point untuk device register dan publish data.
* Cheatsheet Table

Fungsi        | Url API            | Method
------------- | -------------      | -------------
Register      | /service/register/ | GET
Publish Data  | /service/publish/  | POST

* Register API
  Register API digunakan untuk mendaftarkan device (humidifier) ke server.
  Setelah device terdaftar, maka device bisa duhubungkan atau __linking__ ke akun RS.
* Publish data API
