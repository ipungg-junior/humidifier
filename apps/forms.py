from django import forms
from .models import ClientAccount, DeviceSession, ClientDevice

class LoginForm(forms.ModelForm):
  
    class Meta:
        model = ClientAccount   
        fields = ('email', 'password')
        widgets = {
            'email' : forms.EmailInput(attrs={'placeholder' :'Email', 'class': 'form-control', 'id':'email'}),
            'password' : forms.PasswordInput(attrs={'placeholder' :'Password', 'autocomplete': 'password', 'class': 'form-control', 'id': 'password'})
        }

class RegisterForm(forms.ModelForm):

    class Meta:
        model = ClientAccount   
        fields = ('email', 'password')
        widgets = {
            'email' : forms.EmailInput(attrs={'id':'email-input','placeholder' :'Email', 'class': 'form-control'}),
            'password' : forms.PasswordInput(attrs={'id':'password-input','placeholder' :'Password', 'autocomplete': 'password', 'class': 'form-control'}),            
        }


class AddDeviceForm(forms.ModelForm):

    class Meta:
        model = ClientDevice   
        fields = ('deviceID', 'deviceRoom')
        widgets = {
            'deviceID' : forms.TextInput(attrs={'placeholder' :'Device ID', 'class': 'form-control'}),
            'deviceRoom' : forms.TextInput(attrs={'placeholder' :'Ruangan', 'class': 'form-control'}),
        }

class DeviceSessionSetupForm(forms.Form):
    user = forms.CharField()