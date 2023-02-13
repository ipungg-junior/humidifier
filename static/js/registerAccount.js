var inEmail = document.getElementById('email-input');
var inPass = document.getElementById('password-input');
var loading = document.getElementById('loading');
var inKonfirPass = document.getElementById('konfirmasi-password-input');
var btnRegister = document.getElementById('btn-register');
var csrf_token = document.getElementsByName('csrfmiddlewaretoken')[0].value;


btnRegister.addEventListener('click', function(){
    loading.style.display = 'flex';
    var emailVal = inEmail.value;
    var passVal = inPass.value;
    var konfirPassVal = inKonfirPass.value;
    if (passVal == konfirPassVal){
        if(passVal.length < 8){
            alert("Buat password minimal 8 karakter.");
            loading.style.display = 'none';
        }else{
            
            var res = fetch("/account/register/", {
                method: "POST",
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': csrf_token,
                }, 
                body: JSON.stringify({
                    "email": emailVal,
                    "password": passVal,
                })
              }).then(res => {
                if(res.status == 200){
                    loading.style.display = 'none';
                    window.location.href = '/monitoring/';
                }
                if(res.status == 403){
                    loading.style.display = 'none';
                    alert('Gagal mendaftarkan, hubungi administrator. [CSRF_TOKEN]');
                }
                if(res.status == 401){
                    loading.style.display = 'none';
                    alert('Gagal mendaftarkan [CODE 401]');
                }
                return res;
            })
            
        }
    }else{
        alert("Password tidak sama.");
        loading.style.display = 'none';
    }
})
