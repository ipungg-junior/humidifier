from datetime import date
import pyrebase

firebaseConfig = {
    'apiKey': "AIzaSyAH_XjzeRBBNyos0Jq525OwGywE9ZnenF8",
    'authDomain': "raihan-eb807.firebaseapp.com",
    'databaseURL': "https://raihan-eb807-default-rtdb.asia-southeast1.firebasedatabase.app",
    'projectId': "raihan-eb807",
    'storageBucket': "raihan-eb807.appspot.com",
    'messagingSenderId': "175773389068",
    'appId': "1:175773389068:web:c6ec30f91e3bea8026f929"
}

class FirebaseDB:
    
    def __init__(self):
        self.__instance = pyrebase.initialize_app(firebaseConfig)
        self.__DB_instance = self.__instance.database()
        

    def registerDevice(self, deviceID, owner):
        import datetime
        tm = str(datetime.datetime.now())
        formDB = {
            'deviceID': deviceID,
            'registered_at': tm,
            'registered_by': owner
        }
        try:
            self.__DB_instance.child('device_registered').child(deviceID).set(formDB)
            return True
        except:
            return False


    def getDeviceInfo(self, deviceNum):
        try:
            deviceInfo = self.__DB_instance.child('device_registered').child(deviceNum).get().val()
            queryDict = {}
            for key, value in deviceInfo.items():
                queryDict[key] = value
            return queryDict
        except:
            return None


    def updateDevice(self, deviceID, changeKey, changeValue):
        try:
            
            device = self.getDeviceInfo(deviceID)
            if (device is not None):
                try:
                    self.__DB_instance.child('device_registered').child(deviceID).update({changeKey: changeValue})
                    return True
                except:
                       return False
        
            return False
        except:
            return False