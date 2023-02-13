import json
from random import randint
from apps.models import ClientDevice, DeviceSession, DeviceUsage, ClientAccount, StandaloneDeviceSession
from .utils import convertToList
from apps.client_manager import ClientManager
from .myFirebase import FirebaseDB
from django.core.exceptions import ValidationError
from django.core.validators import validate_email

firebaseDB = FirebaseDB()
clientManagerAccount = ClientManager()

class ManagementDevice:


    @staticmethod
    def registerDeviceOnFirebase(deviceID, owner='default'):
        status = firebaseDB.registerDevice(deviceID, owner)
        return status

    @staticmethod
    def checkDeviceOnFirebase(deviceID):
        status = firebaseDB.getDeviceInfo(deviceID)
        return status


    @staticmethod
    def getAllDevice(auth_user):
        '''
            Fungsi untuk mengambil semua device yang ada dalam local database,
            param di isi dengan req.user_auth
        '''
        try:
            query_device = ClientDevice.objects.all()                            
            return query_device
        except:
            return None


 
    @staticmethod
    def linkingDevice(user, data):
        '''
            Return true jika linking berhasil, false jika gagal.
        '''
        deviceID = str(data)
        owner = str(user)
        
        # Get data info from firebase server, is that registered?
        status = firebaseDB.getDeviceInfo(deviceID)
        if (status is not None):
            
            #firebase update owner
            sts = firebaseDB.updateDevice(deviceID, 'registered_by', owner)
            
            #firebase success update
            if (sts):

                try:
                    # Jika try berhasil maka device sudah di linking (return false)
                    obj_deviceClient = ClientDevice.objects.get(deviceID=deviceID)
                    return False
                except:
                    # Jika try gagal maka device akan di proses linking (return true)
                    new_device = ClientDevice.objects.create(
                        deviceID=int(deviceID),
                        deviceRoom=str(''),
                        clientAccount=user
                    )
                    new_device.save()
                    return True
            else:
                return False
        
        return False


    @staticmethod
    def record(sessionID, data):
        '''
            Fungsi untuk menyimpan data dari request dari Esp32.
        '''

        clientDevice_object = ClientDevice.objects.get(
            deviceID=int(data['deviceID']))
        clientAccount_object = ClientAccount.objects.get(
            email=clientDevice_object.clientAccount.email)
        session_object = DeviceSession.objects.get(
            deviceID=clientDevice_object, sessionID=sessionID)

        try:
            record = DeviceUsage.objects.create(
                clientAccount=clientAccount_object,
                deviceID=clientDevice_object,
                sessionID=session_object,
                suhu_chamber_a=int(data['suhu_chamber_a']),
                suhu_chamber_b=int(data['suhu_chamber_b']),
                suhu_output_pasien=int(data['suhu_output_pasien']),
                suhu_heater_plate=int(data['suhu_heater_plate']),
                arus_heater_plate=int(data['arus_heater_plate']),
                arus_heater_wire=int(data['arus_heater_wire']),
                code=int(data['code'])
            )
            # save last session
            session_object.stop_session = record.date_record
            session_object.save()
            record.save()
        except:
            # Tidak ada sesi terbuka, maka data akan disimpan pada StandaloneSession
            print('Tidak ditemukan sesi, simpan standalone')
            unknownSession = StandaloneDeviceSession(
                clientAccount=clientAccount_object,
                deviceID=clientDevice_object,
                suhu_chamber_a=int(data['suhu_chamber_a']),
                suhu_chamber_b=int(data['suhu_chamber_b']),
                suhu_output_pasien=int(data['suhu_output_pasien']),
                suhu_heater_plate=int(data['suhu_heater_plate']),
                arus_heater_plate=int(data['arus_heater_plate']),
                arus_heater_wire=int(data['arus_heater_wire']),
                code=int(data['code'])
            )
            unknownSession.save()
 
    @staticmethod
    def checkSession(deviceID):
        '''
            Fungsi untuk mengambil seluruh session device dengan parameter obj device,
            return (true, obj_session) atau (false, none)
        '''
        deviceclient_object = ClientDevice.objects.get(deviceID=deviceID)
        all_session = DeviceSession.objects.filter(
            deviceID=deviceclient_object, status=True)
        if all_session.count() == 0:
            return False, None
        if all_session.count() > 0:
            return True, all_session[0]

    @staticmethod
    def stopSession(deviceID):
        device = ClientDevice.objects.get(deviceID=deviceID)
        session_device = DeviceSession.objects.get(
            deviceID=device, status=True)
        device.status = False
        device.save()
        session_device.status = False
        session_device.save()



    @staticmethod
    def startSession(deviceID, sessionID):
        try:
            # Ambil obj device
            device = ClientDevice.objects.get(deviceID=deviceID)
            # Buat obj baru di tabel DeviceSession (sesi baru)
            d_session = DeviceSession(
                sessionID=sessionID,
                deviceID=device,
                status=True
            )
            d_session.save()
            # ubah status device menjadi true karna digunakan
            device.status = True
            device.save()
            return True
        except:
            return False



    @staticmethod
    def disconnectedDevice(deviceID):
        clientdevice_obj = ClientDevice.objects.get(deviceID=int(deviceID))
        clientdevice_obj.status = False
        clientdevice_obj.save()


    @staticmethod
    def getDeviceInfo(user, deviceID):
        device = ClientDevice.objects.get(deviceID=deviceID)
        if (device.clientAccount == user):
            return device
        else:
            return None


    @staticmethod
    def showHistory(clientAccount, deviceID):
        try:
            clientdevice_obj = ClientDevice.objects.get(
                deviceID=deviceID, clientAccount=clientAccount)
            all_session = DeviceSession.objects.filter(
                deviceID=clientdevice_obj)
            return True, all_session
        except:
            return False, None

    @staticmethod
    def showallHistory(deviceID):
        try:
            all_session = DeviceSession.objects.all()
            return True, all_session
        except:
            return False, None

 

class ManagementAccount:

    @staticmethod
    def create_user(req):
        '''
            Fungsi untuk membuat user baru dengan param (email, password), jika berhasil return objek user,
            jika gagal maka return None
        '''
        try:
            x = (req.body.decode('utf-8'))
            x = json.loads(x)
            email=validate_email(x['email'])
            password=x['password']
            new_user = clientManagerAccount.create_user(email=x['email'], password=password)
            return new_user
        except ValidationError:
            return None
