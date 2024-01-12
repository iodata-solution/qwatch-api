;(function($){

	//vrbk
	jQuery.fn.panoramaViewer=function(optUser){

		//options
		const optDef={
			src			: "",
			rotationY	: 0
		};
		const opt=$.extend(optDef,optUser);

		//canvas
		const myCanvas=$(this);
		var cv=new Canvas();
		function Canvas(){
			this._id		=myCanvas.attr("id");
			this._box		=myCanvas.closest(".canvas_box");
			this._loader	=myCanvas.closest(".canvas_box").find(".canvas_loader");
			this._start		=myCanvas.closest(".canvas_box").find(".canvas_start");

			this._w			=this._box.width();
			this._h			=this._box.height();
		}
		myCanvas.attr({
			'width'  : cv._w,
			'height' : cv._h
		});

		//three.js
		var renderer;
		var scene;
		var camera;
		var controls;

		//group
		var grpWorld;
		var grpBase;

		//loader
		var loader={};

		//timer
		var timerLoading;

		//ジャイロセンサー確認
		var isGyro=false;
		if((window.DeviceOrientationEvent)&&('ontouchstart' in window)){
			console.log('isGyro=true');
			isGyro=true;
		}

		//PCなど非ジャイロ
		if(!isGyro){
			setCanvas();

		//一応ジャイロ持ちデバイス
		}else{
			//ジャイロ動作確認
			var resGyro=false;
			window.addEventListener("deviceorientation",doGyro,false);
			function doGyro(){
				resGyro=true;
				window.removeEventListener("deviceorientation",doGyro,false);
			}

			//数秒後に判定
			setTimeout(function(){
				//ジャイロが動いた
				if(resGyro){
					setCanvas();

				//ジャイロ持ってるくせに動かなかった
				}else{
					//iOS13+方式ならクリックイベントを要求
					if(typeof DeviceOrientationEvent.requestPermission==="function"){
						//ユーザアクションを得るための要素を表示
						cv._start.show();
						cv._start.on("click",function(){
							cv._start.hide();
							DeviceOrientationEvent.requestPermission().then(res => {
								//「動作と方向」が許可された
								if(res==="granted"){
									setCanvas();
								//「動作と方向」が許可されなかった
								}else{
									isGyro=false;
									setCanvas();
								}
							});
						});

					//iOS13+じゃない
					}else{
						//早くアップデートしてもらうのを祈りながら諦める
						isGyro=false;
						setCanvas();
					}
				}
			},300);
		}


		///////////////////////////////////////////////////////////////////////////////////////////
		// Canvas
		///////////////////////////////////////////////////////////////////////////////////////////

		function gridA() {
			//scene.add(new THREE.AxesHelper(10));    // 座標軸を表示 　X赤 Y緑 Z青
			scene.add(new THREE.AxesHelper(5));    // 座標軸を表示 　X赤 Y緑 Z青
			//scene.add(new THREE.GridHelper(10, 3));  //グリッド表示 size,divisions
			scene.add(new THREE.GridHelper(1, 100));  //グリッド表示 size,divisions
		}

		function cubeA() {
			// 立方体
			const cube = new THREE.Mesh(
				//new THREE.BoxGeometry(),
				new THREE.BoxGeometry( 0.5, 0.5, 0.5 ),		//近すぎて、カメラが被る？
				//new THREE.BoxGeometry( 0.001, 0.001, 0.001 ),
				//new THREE.BoxGeometry( 1000, 1000, 1000 ),
				new THREE.MeshStandardMaterial({ color: 0x00ff00 }));
			cube.castShadow = true;
			cube.position.set(0, 0, 0);
			scene.add(cube);
		};
		function flower00(){
			// 画像を読み込む
			var texture = new THREE.TextureLoader().load('images/sunflower.jpg',
			(tex) => { // 読み込み完了時
				// 縦横比を保って適当にリサイズ
				const w = 5;
				const h = tex.image.height/(tex.image.width/w);
				// 平面
				const geometry = new THREE.PlaneGeometry(1, 1);
				const material = new THREE.MeshPhongMaterial( { map:texture } );
				const plane = new THREE.Mesh( geometry, material );
				plane.scale.set(w, h, 1);
				plane.position.set( 0, 0, -3
					);
				plane.rotation.set( 0, 0, 0);
				scene.add( plane );
			});
		}

		function facets14() {
			const ang000=0;
			const ang045=Math.PI/4;
			const ang090=Math.PI/2;
			const ang135=Math.PI*3/4;
			const ang180=Math.PI;
			const ang225=-Math.PI*3/4;
			const ang270=-Math.PI/2;
			const ang315=-Math.PI/4;
			const sin000=Math.sin(ang000);
			const sin045=Math.sin(ang045);
			const sin090=Math.sin(ang090);
			const sin135=Math.sin(ang135);
			const sin225=Math.sin(ang225);
			const sin180=Math.sin(ang180);
			const sin270=Math.sin(ang270);
			const sin315=Math.sin(ang315);
			const cos000=Math.cos(ang000);
			const cos045=Math.cos(ang045);
			const cos090=Math.cos(ang090);
			const cos135=Math.cos(ang135);
			const cos180=Math.cos(ang180);
			const cos225=Math.cos(ang225);
			const cos270=Math.cos(ang270);
			const cos315=Math.cos(ang315);
			const r=5;
			//const r=4.9;
			const imgs_ini = [
				'init01.jpg','init02.jpg','init03.jpg','init04.jpg',
				'init05.jpg','init06.jpg','init07.jpg','init08.jpg',
				'init09.jpg','init10.jpg','init11.jpg','init12.jpg',
				'init13.jpg','init14.jpg',];
			const imgs = [
				'01.jpg','02.jpg','03.jpg','04.jpg',
				'05.jpg','06.jpg','07.jpg','08.jpg',
				'09.jpg','10.jpg','11.jpg','12.jpg',
				'13.jpg','14.jpg',];
			const geos = [ // 横、高、奥、角度は難しい
				[ r*cos090, r*sin090, r*cos270, ang090, ang000, 0],  // 上 OK
				[ r*cos090, r*sin045, r*cos225, ang045, ang000, ang000],    // 前上 ok
				[ r*cos045, r*sin045, r*cos090, ang090, ang315, ang090],    // 右上 ok
				[ r*cos270, r*sin045, r*cos045, ang315, ang180, ang000],    // 後上 ok
				[ r*cos135, r*sin045, r*cos270, ang270, ang135, ang090],    // 左上 ok
				[ r*cos090, r*sin000, r*cos180, ang000, ang000, 0],  // 前 ok
				[ r*cos000, r*sin000, r*cos090, ang000, ang270, 0],  // 右 ok
				[ r*cos270, r*sin000, r*cos000, ang000, ang180, 0],    // 後 ok
				[ r*cos180, r*sin000, r*cos270, ang000, ang090, 0],    // 左 ok
				[ r*cos090, r*sin315+r*0.2, r*cos135, ang315, ang000, ang000],    // 前下 ok
				[ r*cos315, r*sin225+r*0.2, r*cos090, ang270, ang315, ang270],    // 右下 OK
				[ r*cos270, r*sin315+r*0.2, r*cos045, ang045, ang180, ang000],    // 後下 OK 
				[ r*cos135, r*sin225+r*0.2, r*cos270, ang090, ang135, ang270],    // 左下 OK
				[ r*cos090, r*sin270, r*cos090, ang270, ang000, 0]];  // 下 OK
		
			//var width = 5*2;
			//var height=3.75*2;
			const width = 10;
			const height = 1080*width/(1920);       //3種とも比率は同じ
			var loader = new THREE.TextureLoader();
			const d = './images/';		// srt power of two (640x360). Resized to ????x???? 必ずリサイズ 
			//const d = './images/1920x1080/init';    // 1080/1920=0.5625
			//const d = './images/1280x0720/init';    // 0720/1280=0.5625
			//const d = './images/0640x0360/init';    // 0360/640=0.5625

			let panels=[];
			geos.forEach(function (g, i) {
				panels[i] = new THREE.Mesh(
					new THREE.PlaneGeometry(width, height, 1, 1),
					new THREE.MeshBasicMaterial({
						map:loader.load(d+imgs[i]),
						//side:THREE.DoubleSide     //手前を透過しない
					}));
				panels[i].position.set(g[0], g[1], g[2]);
				panels[i].rotation.set(g[3], g[4], g[5]);
				scene.add(panels[i]);
			});
		};
		

		function setCanvas(){
			if(!Detector.webgl){Detector.addGetWebGLMessage();}
			cv._loader.show();

			renderer=new THREE.WebGLRenderer({
				canvas    : document.querySelector('#'+cv._id),
				antialias : true,
				alpha     : true
			});

			renderer.setPixelRatio(window.devicePixelRatio);
			renderer.setSize(cv._w,cv._h);

			scene=new THREE.Scene();
			grpWorld=new THREE.Group();
			grpBase=new THREE.Group();


			scene.background = new THREE.Color(0x7777FF); // 背景色

			//スマホなどジャイロセンサーが有効なときはDeviceOrientationControls
			if(isGyro){
				camera=new THREE.PerspectiveCamera(60,cv._w/cv._h,1,20000);
				camera.position.set(0,0,0.01);
				//camera.position.set(0,0,10);	//これで動きが見えた　中央のオブジェクト内にカメラがあったから？
				camera.lookAt(new THREE.Vector3(0,0,0));

				controls=new THREE.DeviceOrientationControls(camera);
				controls.connect();
				controls.update();

			//PCなどジャイロセンサーがない場合はOrbitControlsのみ
			}else{
				camera=new THREE.PerspectiveCamera(60,cv._w/cv._h,1,20000);
				camera.position.set(0,0,0.01);
				//camera.position.set(0,0,10);	//100倍にすると見えた。これで動きが見えた
				camera.lookAt(new THREE.Vector3(0,0,0));
				controls=new THREE.OrbitControls(camera,renderer.domElement);

				controls.autoRotate		=false;
				//controls.autoRotate		=true;	//早すぎる
				controls.enableRotate	=true;
				//controls.rotateSpeed	=-0.05;
				controls.rotateSpeed	=-0.1;		//少ないとマウス操作じゃ動かない
				controls.enableDamping	=true;
				controls.dampingFactor	=0.1;
				//controls.enableZoom		=false;
				controls.enableZoom		=true;	//false;
				controls.enablePan		=false;
			}

// ライト	これがないとセンターキューや写真が見えない
var light = new THREE.AmbientLight( 0xffffff );
scene.add( light );


			///////////////////////////////////////////////////////////////////////
			// 3Dオブジェクト配置
			///////////////////////////////////////////////////////////////////////

			//base
			/*
			var gmBase=new THREE.SphereBufferGeometry(1000,60,40);
			gmBase.scale(-1,1,1);
			var mtrBase;
			if(opt.src==""){
				mtrBase=new THREE.MeshNormalMaterial();
			}else{
				loader[opt.src]=false;
				var texture=new THREE.TextureLoader().load(opt.src,function(){
					loader[opt.src]=true;
				});

				texture.minFilter=texture.magFilter=THREE.LinearFilter;
				texture.mapping=THREE.UVMapping;

				mtrBase=new THREE.MeshBasicMaterial({
					map : texture
				});
			}
			var base=new THREE.Mesh(gmBase,mtrBase);
			base.rotation.y=(opt.rotationY*Math.PI/180);
			grpBase.add(base);

			grpWorld.add(grpBase);
			scene.add(grpWorld);
			*/
			/*
			*/
			//中央のボックス
			/*
			var base = new THREE.Mesh(
				//new THREE.BoxGeometry(),
				new THREE.BoxGeometry( 0.5, 0.5, 0.5 ),
				new THREE.MeshStandardMaterial({ color: 0x00ff00 }));
			base.castShadow = true;
			base.position.set(0, 0, 0);
			scene.add(base);
			*/
			//gridA();
			//cubeA();
			//flower00();
			facets14();


			//checkLoading
			var check_sec=500;
			var check_cnt=0;
			var check_limit=30000;

			checkLoading();
			function checkLoading(){
				var flag_loading=false;
				Object.keys(loader).forEach(function(key){
					if(!loader[key]){
						flag_loading=true;
					}
				},loader);

				//読み込み中
				if(flag_loading){
					timerLoading=setTimeout(function(){
						check_cnt+=check_sec;
						if(check_cnt<check_limit){
							checkLoading();
						}else{
							//エラー処理など
							alert("読み込みに失敗しました。");
						}
					},check_sec);

				//読み込み完了
				}else{
					clearTimeout(timerLoading);
					cv._loader.hide();

					runAnimate();
				}
			}

			//runAnimate
			function runAnimate(){
				controls.update();

				renderer.render(scene,camera);
				requestAnimationFrame(runAnimate);
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////
		// Window Resize
		///////////////////////////////////////////////////////////////////////////////////////////

		var timerResize=false;
		$(window).on("resize",function(){
			if(timerResize!==false){
				clearTimeout(timerResize);
			}
			timerResize=setTimeout(function(){
				resizeCanvas();
			},500);
		});

		function resizeCanvas(){
			cv._w=cv._box.width();
			cv._h=cv._box.height();
			myCanvas.attr({
				'width'  : cv._w,
				'height' : cv._h
			});

			renderer.setPixelRatio(window.devicePixelRatio);
			renderer.setSize(cv._w,cv._h);
			camera.aspect=cv._w/cv._h;
			camera.updateProjectionMatrix();
		}

	};

})(jQuery);