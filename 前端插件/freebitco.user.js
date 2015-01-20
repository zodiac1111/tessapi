//刷新简历
$(document).ready(function() {
	setInterval(function() {
		check();
	}, 5 * 1000);
	setInterval(function() {
		error();
	}, 5 * 1000);

});
function check() {
	//console.log("检查 " + Date());
	var a = 0;
	var ret;
	var isPlay = $("#free_play")[0].style.cssText != "display: none;";
	var cap = $("#free_play_captcha_image");
	if (isPlay) {
		console.log("开始post " + Date());
		post(cap[0].src);
	}
}

function post(pic_url) {
	//alert("post")
	// ajax检验验证码
	$.ajax({
		type : "post",
		url : "http://127.0.0.1:8080/cgi-bin/api.cgi", // 验证码识别接口api.目前运行在本机上
		contentType : "application/x-www-form-urlencoded; charset=utf-8",
		dataType : "text",
		data : pic_url, //传给识别器图片链接,其自己下载后识别,再返回.
		success : function(result, textStatus) {
			//$("#return")[0].value = result;
			$("#free_play_captcha_answer")[0].value = result;
			//ajax 发送验证码
			$("#free_play_form_button")[0].click();
			console.log("post完成 " + result + Date());
		},
		error : function() {//失败
			console.log("!!!!服务器通讯错误 "  + Date());
		}
	});
	//return "1234";
}

function error() {
	var notOk = $("#free_play_error")[0].textContent == "Incorrect number entered";

	if (notOk) {
		var cap = $("#free_play_captcha_image");
		post(cap[0].src);
	}
}
