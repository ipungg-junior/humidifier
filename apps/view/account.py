from django.shortcuts import redirect, HttpResponse, render
from django.contrib.auth import authenticate, login, logout
from django.views import View
from apps.forms import LoginForm, RegisterForm
from apps.services.ResponsesWorker import DefaultResponse
from apps.services.DBManagement import ManagementAccount



class Account(View):
    template = ''
    context = ''

    def get(self, req):
        if (self.context=='login'):
            if (req.user.is_anonymous):    
                form = LoginForm()
                response = DefaultResponse.defaultResponse(req, 'login.html', context={'form':form})
                return response
            else:
                return redirect('/monitoring/')

        if (self.context=='register'):
            if (req.user.is_anonymous):    
                form = RegisterForm()
                response = DefaultResponse.defaultResponse(req, 'register.html', context={'form':form})
                return response
            else:
                return redirect('/monitoring/')

        if (self.context=='logout'):
            logout(req)
            return redirect('/')



    def post(self, req):
        if (self.context=='login'):
            user = authenticate(req, username=req.POST['email'], password=req.POST['password'])
            if user is not None:
                login(req, user)
                print(f'{user} berhasil login.')
                return redirect('/monitoring/')
            else:
                print('{user} : Invalid password')
                form = LoginForm() 
                return render(req, 'login.html', context={
                    'form': form
                })

        if (self.context=='register'):
            new_user = ManagementAccount.create_user(req)
            if new_user is not None:
                login(req, new_user)
                return redirect('/monitoring/')
            else:
                form = RegisterForm() 
                return render(req, 'register.html', context={
                    'form': form
                })

        else:
            return HttpResponse(code=401)