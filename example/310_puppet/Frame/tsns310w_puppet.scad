echo("TS-NS310W_PUPPET");

module cam(diameter=70,add_h=20)
{
    union()
    {
        difference()
        {
            translate([0,0,0])sphere(d=diameter);
            translate([0,0,-0.01])mirror([0,0,1])cylinder(h=diameter/2,d=diameter,center=false);
        }
        translate([0,0,-diameter/2-add_h])cylinder(h=diameter/2+add_h,d=diameter,center=false);
    }
}
//cam(diameter=70,add_h=-20);

module puppet_hand()
{
    //U型パーツ
    //if(0)
    intersection()
    {
        translate([0.0,0.0,-72/2]) 
        difference()
        {
            cam(90,-25);
            cam(82,-20);
        }
        cube([20,150,200],center=true);
    }

    //サーボ位置
    //if(0)
    difference()
    {   
        translate([0,0,-72/2+25]) 
        union()
        {
            translate([0, 43,6])    rotate([0,0,-10])cube([13,31,3],center=true);
            translate([0,-48,-40.6])rotate([0,0,+10])cube([13,21,3],center=true);
        }
		#translate([2, 61.5,-8.11])rotate([0,0,80])SG90arm(180);
		#translate([3,-61.5,-48.5])rotate([0,180,100])SG90arm(180);
        #translate([0.0,0.0,-72/2])cam(72+5+5,-20);
    } 

    //円柱状マウント
    //if(0)
    intersection()
    {
        difference()
        {
            union()
            {
                translate([0,0,32])scale([0.5,1.0,1.2])rotate([0,-90,0])cylinder(d=60,h=20);
                translate([0,0,-5+5]) cylinder(h=10+2,d1=40,d2=30,center=true);
            }
            translate([0.0,0.0,-72/2]) sphere(d=72, $fa=5, $fs=0.1); 
        }
        translate([0+5.0,0.0,0]) cube([20+10,150,200],center=true); //印刷面を作る
    }

}
puppet_hand();



module SG90arm(ang=0)
{
    translate([16.6,-5.9,-7.0])rotate([-90,0,90]) SG90();
    rotate([0,0,ang])servo_arm1();
}
//SG90arm(180);

module SG90(){
    color("Blue",0.8){ 
        cube([11.8,22.7,22.5]); //ケース
        difference(){    
            translate([0,4.3,-4.7])cube([11.8,2.5,31.9]);//固定部 
            translate([5.9,7.3,-2.4])rotate([90,0,0])cylinder(r=1,h=3.5,$fn=100);//ネジ穴
            translate([5.9,7.3,24.9])rotate([90,0,0])cylinder(r=1,h=3.5,$fn=100);//ネジ穴
            translate([5.25,4,25.6])cube([1.3,3.2,2.3]);//欠き切り
            translate([5.25,4,-4.7])cube([1.3,3.2,2.3]);//欠き切り
        }
        translate([5.9,0,16.6]) rotate([90,0,0])cylinder(r=5.9,h=4,$fn=100);//凸部
    }
    color("White",0.8){
        translate([5.9,-4,16.6])rotate([90,0,0])cylinder(r=2.3,h=3.2,$fn=100);//回転軸
    }
}
//SG90();

//サーボアーム
module servo_arm1()
{
    a=360/(21*2); //山と谷の挟み角(21山)
    r1=5.250/2;   //山の半径
    r2=4.502/2;   //谷の半径

    //difference()  //今回の用途によりコメント化
    {
        color("white")
        union()
        {
            //if(0)
            hull()
            {
                translate([14.5,0,0])cylinder(h=1.5,d=4.3,$fn=30); //サーボ軸側
                cylinder(h=1.5,d=6.9,$fn=30);  //アーム先端側
            }
            //if(0)
            mirror([0,0,1])cylinder(h=2.3,d=6.7,$fn=30);//スプラインの周囲
        }
        mirror([0,0,1])linear_extrude(2.3+0.1)
            polygon([for(i=[0:21*2-1])[(i%2?r2:r1)*cos(a*i),(i%2?r2:r1)*sin(a*i)]]); //Gear
        cylinder(d=2.5,h=5,center=true,$fn=30);   //サーボ固定穴
        for(i=[0:5])translate([i*(10.2/5)+4.5,0,0])cylinder(d=1.8,h=20,center=true,$fn=10); //ネジ穴
    }
}
//servo_arm1();


