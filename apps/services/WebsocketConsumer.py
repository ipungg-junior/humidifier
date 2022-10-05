import json, random
from channels.generic.websocket import AsyncWebsocketConsumer
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync


class PrivateWebsocket(AsyncWebsocketConsumer):

    deviceID = ''
    async def connect(self):

        self.deviceID = self.scope['url_route']['kwargs']['deviceID']
        await self.channel_layer.group_add(
            f'device_{self.deviceID}',
            self.channel_name
        )
        await self.accept()


    async def disconnect(self, close_code):
        # Leave room group
        await self.channel_layer.group_discard(
            f'device_{self.deviceID}',
            self.channel_name
        )

    async def device_incoming_post(self, event):
        data = event['data']
        # Send message to WebSocket
        await self.send(
            text_data=json.dumps({
            'data': json.loads(data)
        }))



class WebsocketWorker:

    @staticmethod
    def sendToWebsocket(channel, message):

        ## Get layer channel
        layer = get_channel_layer()
        ## Send streaming PrivateWebsocket (spesific group, no broadcast)
        async_to_sync(layer.group_send)(
            f'device_{channel}', 
        {
            "type": "device_incoming_post",
            "data": json.dumps(message)
        })