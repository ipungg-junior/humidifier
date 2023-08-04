from statistics import mode
from django.db import models
from django.contrib.auth.models import AbstractBaseUser, PermissionsMixin
from django.utils import timezone
from django.utils.translation import gettext_lazy as _
from .client_manager import ClientManager


class ClientAccount(AbstractBaseUser, PermissionsMixin):
    email = models.EmailField(_('email address'), unique=True)
    is_staff = models.BooleanField(default=False)
    is_active = models.BooleanField(default=True)
    date_joined = models.DateTimeField(default=timezone.now)
    

    # Client Manager Reuirements
    USERNAME_FIELD = 'email'
    REQUIRED_FIELDS = []
    objects = ClientManager()

    def __str__(self):
        return self.email


 
class ClientDevice(models.Model):
    '''
        ClientDevice adalah table untuk menyimpan perangkat yang terdaftar pada RS.
        setiap RS harus mendaftarkan perangkatnya agar terhubung dengan akun RS
    '''

    deviceID = models.CharField(blank=True, max_length=18)
    clientAccount = models.ForeignKey(ClientAccount, on_delete=models.CASCADE, null=True)
    status = models.BooleanField(default=False)
    deviceRoom = models.CharField(max_length=16, blank=True)
    date_joined = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f'{self.deviceID} milik {self.clientAccount}'



class StandaloneDeviceSession(models.Model):
    '''
        Database ini menyimpan device yang request, tapi tidak ada session yang terbuka.
        Jika pihak RS tidak membuat sesi baru, maka data request dari Esp akan disimpan disini.
    '''
    clientAccount = models.ForeignKey(ClientAccount, on_delete=models.CASCADE, null=True)
    deviceID = models.ForeignKey(ClientDevice, on_delete=models.CASCADE, null=True)
    suhu_chamber_a = models.IntegerField(null=False, default=0)
    suhu_chamber_b = models.IntegerField(null=False, default=0)
    suhu_output_pasien = models.IntegerField(null=False, default=0)
    suhu_heater_plate = models.IntegerField(null=False, default=0)
    arus_heater_plate = models.IntegerField(null=False, default=0)
    arus_heater_wire = models.IntegerField(null=False, default=0)
    date_record = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f'{self.deviceID}'



class DeviceSession(models.Model):
    '''
        Tabel ini berupa daftar sesi dari semua perangkat.
        Pihak RS membuat data sesi baru untuk memonitoring device.
        setiap device session akan memiliki banyak data penggunaan (OneToMany -> DeviceUsage)
    '''

    sessionID = models.CharField(max_length=24)
    deviceID = models.ForeignKey(ClientDevice, on_delete=models.CASCADE)
    status = models.BooleanField(default=True)
    deviceRoom = models.CharField(max_length=16, blank=True)
    start_session = models.DateTimeField(auto_now_add=True)
    stop_session = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f'{self.sessionID}'



class DeviceUsage(models.Model):
    '''
        DeviceUsage terhubung dengan DeviceSession agar data urut sesuai dengan sessionID.
    '''

    clientAccount = models.ForeignKey(ClientAccount, on_delete=models.CASCADE, null=True)
    deviceID = models.ForeignKey(ClientDevice, on_delete=models.CASCADE, null=True)
    sessionID = models.ForeignKey(DeviceSession, on_delete=models.CASCADE, null=True)
    suhu_chamber_a = models.IntegerField(null=False, default=0)
    suhu_chamber_b = models.IntegerField(null=False, default=0)
    suhu_output_pasien = models.IntegerField(null=False, default=0)
    suhu_heater_plate = models.IntegerField(null=False, default=0)
    arus_heater_plate = models.IntegerField(null=False, default=0)
    arus_heater_wire = models.IntegerField(null=False, default=0)
    code = models.SmallIntegerField(null=True, default=0)
    date_record = models.DateTimeField(auto_now_add=True)
    

    def __str__(self):
        return f'{self.sessionID} -- {self.deviceID}'



class RegisterDeviceSegmentation(models.Model):
    deviceID = models.IntegerField(unique=True)
    status = models.CharField(max_length=14, blank=False, default='UNREGISTERED')
    date_register = models.DateTimeField(auto_now_add=True)
