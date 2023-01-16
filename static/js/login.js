var info = document.getElementById('info');
var em = document.getElementById('email');
var ps = document.getElementById('password');
var btn = document.getElementById('btn-login');
var csrf_token = document.getElementsByName('csrfmiddlewaretoken')[0].value;
btn.addEventListener('click', function(){
    var ema = em.value;
    var pss = ps.value;
    loginAuth(ema, pss);
})

document.onkeydown=function(evt){
    var keyCode = evt ? (evt.which ? evt.which : evt.keyCode) : event.keyCode;
    if(keyCode == 13)
    {
        var ema = em.value;
        var pss = ps.value;
        loginAuth(ema, pss);
    }
}

function loginAuth(email, pass){

    fetch("/account/login/", {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
        }, 
        body: JSON.stringify({
            'email': email,
            'password': pass
        })
      }).then(res => {
        if(res.status == 200){
            info.style.display = 'initial';
            info.style.backgroundColor = 'rgb(74, 138, 84)';
            info.textContent = 'Login Berhasil.';
            window.location.href = '/monitoring/';

        }
        else{
            info.style.display = 'initial';
            info.style.backgroundColor = 'rgb(230, 29, 29)';
            info.textContent = 'Login Gagal.';
            ps.value = '';
        }
        setTimeout(function(){
            info.style.display = 'none';
        },3000)
      });
}