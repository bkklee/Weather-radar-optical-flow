//[Package]
//Express
const express = require("express");
//HTML request
var request = require("request");
//Jimp
var Jimp = require("jimp");
//Fs
var fs = require("fs");
//Path
var path = require("path");

//[Init]
const PORT = process.env.PORT || 5000;

var app = express();

//Image size: 400 * 400
//Region we concerned: 400 * 400 (20*20/1)
var region = 400;
var gridNum = 20;
var gridWidth = 20; //400/20
var colors = {0.50:[0,201,253],1.0:[0,143,243],2.0:[59,150,255],3.0:[1,132,66],5.0:[1,168,51],7.0:[0,208,1],10.0:[0,250,6],15.0:[147,255,0],30.0:[224,207,0],50.0:[255,208,0],75.0:[238,176,0],100.0:[238,129,0],150.0:[241,0,4],200.0:[214,0,0],300.0:[197,0,108],300.0:[237,1,238]};

function colorToPrecipitation(r,g,b){
	for(rain in colors){
		if(r > colors[rain][0]-15 && r < colors[rain][0]+15 && g > colors[rain][1]-15 && g < colors[rain][1]+15 && b > colors[rain][2]-15 && b < colors[rain][2]+15){
			return rain;
		}
	}
	return 0;
}

app.get("/", function(req, res){
	Jimp.read('https://www.hko.gov.hk/wxinfo/radars/rad_128_png/2d128iradar_201906012136.jpg', function (err, image) {
		var tmp = "";
		var i,j,x,y;
		for(j=0;j<gridNum;j++){
			for(i=0;i<gridNum;i++){
				var total = 0.0;
				for(x=0;x<gridWidth;x++){
					for(y=0;y<gridWidth;y++){
						var rgb = Jimp.intToRGBA(image.getPixelColor(i*gridWidth+x, j*gridWidth+y));
						total += Number(colorToPrecipitation(rgb.r,rgb.g,rgb.b));
					}
				}
				tmp += String(total/(gridWidth*gridWidth)) + " ";
			}
			tmp += "\n";
		}

		fs.writeFile(path.join(__dirname + "/rainmapnow.txt"), tmp, function(err){
			Jimp.read('https://www.hko.gov.hk/wxinfo/radars/rad_128_png/2d128iradar_201906012148.jpg', function (err, image) {
				tmp = "";
				var i,j,x,y;
				for(j=0;j<gridNum;j++){
					for(i=0;i<gridNum;i++){
						var total = 0.0;
						for(x=0;x<gridWidth;x++){
							for(y=0;y<gridWidth;y++){
								var rgb = Jimp.intToRGBA(image.getPixelColor(i*gridWidth+x, j*gridWidth+y));
								total += Number(colorToPrecipitation(rgb.r,rgb.g,rgb.b));
							}
						}
						tmp += String(total/(gridWidth*gridWidth)) + " ";
					}
					tmp += "\n";
				}

				fs.writeFile(path.join(__dirname + "/rainmap12mins.txt"), tmp, function(err){
					if(err)
						res.send(err);
					else
						res.send("Done!");
				});
			});
		});
	});
});

app.listen(PORT);