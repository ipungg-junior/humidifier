import json, ast
from django.shortcuts import HttpResponse
from matplotlib.font_manager import json_dump
from .JSONServices import JSONServices
from .DBManagement import ManagementDevice
from .Helper import randomNum
from .WebsocketConsumer import WebsocketWorker
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync

class Esp32:

    ''' Class Esp32 berisi fungsi untuk menghandle 
        seluruh request dari device (esp32) '''
 

    @staticmethod
    def registerMachineCode():
        newMachineCode = randomNum(4)
        res = HttpResponse(json.dumps({'machine-code': newMachineCode}))
        return res


    ## Method to handle linking device ke akun konsumen
    @staticmethod
    def linking(req):
        '''
            Kaitkan akun dengan device baru (parameter obj req dan deviceID). 
        '''
         
        try:
            
            data = (req.body).decode('utf-8')
            status = ManagementDevice.linkingDevice(req.user, data)
            if (status):
                return HttpResponse(json.dumps({
                        'status': 1,
                        'flag': 'info',
                        'message': 'Perangkat dikaitkan.',
                    }))

            else:
                return HttpResponse(json.dumps({
                    'status': -1,
                    'flag': 'danger',
                    'message': 'Proses gagal, hubungi Administrator.',
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
        deviceID = str(data['deviceID'])

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
            
 


   
    @staticmethod
    def receive(req):
        '''
            Method ini untuk menghandle request dari ESP32. Parameter (request)
        '''
         
        # Mengambil data sensor yang dikirim pada body request (JSON)
        data = JSONServices.decode(req.body)
        # Mengambil id sesi dari device request body
        sessionID = str(data['sessionID'])

        try:
            # save database
            ManagementDevice.record(sessionID, data)

            try:
                # Send to Websocket for update data on web
                WebsocketWorker.sendToWebsocket(channel=data['deviceID'], message=data)
            except:
                # If websocket failed.
                print('Error : Websocket failed.')
                return HttpResponse(status=404)

            return HttpResponse(status=200)

        except:
            print(f'-- ERROR --')
            print(f' > Session-ID\t: {sessionID}')
            print(f' > Data\t\t: ')
            for key in data.items():
                print(f' {key}')
            print(f'================================================')
            return HttpResponse(status=500)



    @staticmethod
    def disconnected(req):
        body = (req.body).decode('utf-8')
        ManagementDevice.disconnectedDevice(body[9::])
        return HttpResponse(status=200)