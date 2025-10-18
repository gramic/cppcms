{
  "app": {
    "root": ""
  },
  "localization": {
    "locales": [
        "bg_BG.UTF-8"
    ]
  },
  "logging": {
    "level": "debug"
  },
  "security": {
    "content_length_limit": 102400,
    "multipart_form_data_limit": 102400
  },
	"service" : {
		"api" : "http",
		"port" : 8882,
    "ip" : "0.0.0.0"
		// "port" : 8080
	},
	"http" : {
		"script" : "/hello",
     "script_names" : [ "/hello" ]
	},
  "session": {
    "client": {
        "hmac": "md5",
        "hmac_key": "a15e33bbea5d62c91191892035cb4e74874006d1"
    },
    "cookies": {
        "domain": "mladenov.dev",
        "prefix": "docs_bg_session",
        "secure": true
    },
    "expire": "renew",
    "location": "client",
    "timeout": 1800
  }
}
