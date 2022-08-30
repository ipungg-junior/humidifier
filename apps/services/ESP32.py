import json, ast
from django.shortcuts import HttpResponse
from matplotlib.font_manager import json_dump
from .JSONServices import JSONServices
from .DBManagement import ManagementDevice
from .Helper import randomNum
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync

class Esp32:

    ''' Class Esp32 berisi fungsi untuk menghandle 
        seluruh request dari device (esp32) '''
 
    ## Method to handle register device
    @staticmethod
    def register(req):
        '''
            Registrasi Device baru dengan parameter obj req dan deviceID. 
            status -1 -> Unknown Device, 
            status 0 -> Already Registered, 
            status 1 -> Register Success, 
        '''
         
        try:
            data = (req.body).decode('utf-8')
            status = ManagementDevice.newDevice(req.user, data)
            if (status==-1):
                return HttpResponse(json.dumps({
                    'status': -1,
                    'flag': 'danger',
                    'message': 'Device tidak ditemukan.',
                }))
            if (status==0):
                return HttpResponse(json.dumps({
                    'status': 0,
                    'flag': 'warning',
                    'message': 'Device Sudah Terdaftar',
                }))
            if (status==1):
                return HttpResponse(json.dumps({
                        'status': 1,
                        'flag': 'info',
                        'message': 'Berhasil Didaftarkan.',
                    }))

        except KeyError:
            return HttpResponse(json.dumps({
                    'status': -2,
                    'flag': 'danger',
                    'message': 'Internal Server Error',
                }))



    @staticmethod
    def registerSession(req):
        data = JSONServices.decode(req.body)
        deviceID = data['deviceID']

        # buat id sesi baru otomatis (random)
        new_sessionID = deviceID + randomNum(4)
        
        # Meminta sesi baru (otomatis)
        status = ManagementDevice.startSession(deviceID, new_sessionID)
        if (status):
            ret = HttpResponse(json.dumps({'sessionID': new_sessionID}))
            return ret
        else:
            ret = HttpResponse(json.dumps({'eror': 'Device not found.'}))
            ret.status_code = 400
            return ret
            



    ## Method to handle delete device
    def unregister(self):
        pass



    ## Method to handle receive data from API
    @staticmethod
    def receive(req):
 
        # Mengambil data sensor yang dikirim pada body request
        data = JSONServices.decode(req.body)
        # Mengambil id sesi dari device request body
        sessionID = data['sessionID']

        try:
            ManagementDevice.record(sessionID, data)
            layer = get_channel_layer()

            ## Send streaming PrivateWebsocket (spesific group, no broadcast)
            async_to_sync(layer.group_send)(
                f'device_{data["deviceID"]}', 
            {
                "type": "device_incoming_post",
                "data": json.dumps(data)
            })
            return HttpResponse(status=200)
        except:
            return HttpResponse(status=500)



    @staticmethod
    def disconnected(req):
        body = (req.body).decode('utf-8')
        ManagementDevice.disconnectedDevice(body[9::])
        return HttpResponse(status=200)