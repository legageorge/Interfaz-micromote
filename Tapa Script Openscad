// Tapa definitiva Interfaz Micromote
// dimension cilindro externo: diam=48.26 mm

// dimensiones

marg=0.54;
diam_ext=48.26;
diam_pcb=45.72+marg;
diam_int=40.64;

alt_tapa=1;
alt_pcb=1;
alt_sup=1;
alt_int=3.7;
alt_pilas=6.3;
alt_tot=alt_tapa+alt_pcb+alt_sup+alt_int+alt_pilas;

uusb_hole_x=8.8;
uusb_hole_y=5.6;
uusb_hole_z=3;

switch_hole_x=9.6;
switch_hole_y=7;
switch_hole_z=3.5;

tol=0.2;
var1=1;
var2=0.6;

tapa_hole_diam_inf=4;
tapa_hole_diam_sup=2.286;


union(){

	color("red"){

		difference(){

			translate([0,0,alt_tapa/2]) cylinder(h=alt_tapa, r=diam_ext/2 , $fn=500, center=true);

			translate([12.446,17.526,alt_tapa/2]) {cylinder(h=alt_tapa, r1=tapa_hole_diam_inf/2, r2=tapa_hole_diam_sup/2, $fn=500, center=true);}
			translate([-12.446,17.526,alt_tapa/2]) {cylinder(h=alt_tapa, r1=tapa_hole_diam_inf/2, r2=tapa_hole_diam_sup/2, $fn=500, center=true);}
			translate([-12.446,-17.526,alt_tapa/2]) {cylinder(h=alt_tapa, r1=tapa_hole_diam_inf/2, r2=tapa_hole_diam_sup/2, $fn=500, center=true);}
			translate([12.446,-17.526,alt_tapa/2]) {cylinder(h=alt_tapa, r1=tapa_hole_diam_inf/2, r2=tapa_hole_diam_sup/2, $fn=500, center=true);}
		}
	}

	color("orange"){

		intersection(){

			difference(){
				translate([0,0,((alt_pilas+alt_pcb-tol/2)/2)+alt_tapa]) cylinder(h=alt_pilas+alt_pcb-tol/2, r=diam_ext/2, $fn=500, center=true);
				translate([0,0,((alt_pilas+alt_pcb-tol/2)/2)+alt_tapa]) cylinder(h=alt_pilas+alt_pcb-tol/2, r=diam_pcb/2, $fn=500, center=true);					
			}

			union(){
				translate([0,-diam_pcb/2,((alt_tapa+alt_pilas+alt_pcb)/2)]) cube([uusb_hole_x+tol,6,alt_tapa+alt_pilas+alt_pcb], center=true);
				translate([0,diam_pcb/2,((alt_tapa+alt_pilas+alt_pcb)/2)]) cube([switch_hole_x+tol,6,alt_tapa+alt_pilas+alt_pcb], center=true);
			}				
		}
	}

	color("green"){

		translate([12.446,17.526,alt_tapa+alt_pilas/2]) {
			difference(){
				cylinder(h=alt_pilas, r1=tapa_hole_diam_sup/2+var1, r2=tapa_hole_diam_sup/2+var2, $fn=500, center=true);
				cylinder(h=alt_pilas, r=tapa_hole_diam_sup/2, $fn=500, center=true);
			}
		}

		translate([-12.446,17.526,alt_tapa+alt_pilas/2]) {
			difference(){
				cylinder(h=alt_pilas, r1=tapa_hole_diam_sup/2+var1, r2=tapa_hole_diam_sup/2+var2, $fn=500, center=true);
				cylinder(h=alt_pilas, r=tapa_hole_diam_sup/2, $fn=500, center=true);
			}
		}

		translate([-12.446,-17.526,alt_tapa+alt_pilas/2]) {
			difference(){
				cylinder(h=alt_pilas, r1=tapa_hole_diam_sup/2+var1, r2=tapa_hole_diam_sup/2+var2, $fn=500, center=true);
				cylinder(h=alt_pilas, r=tapa_hole_diam_sup/2, $fn=500, center=true);
			}
		}

		translate([12.446,-17.526,alt_tapa+alt_pilas/2]) {
			difference(){
				cylinder(h=alt_pilas, r1=tapa_hole_diam_sup/2+var1, r2=tapa_hole_diam_sup/2+var2, $fn=500, center=true);
				cylinder(h=alt_pilas, r=tapa_hole_diam_sup/2, $fn=500, center=true);
			}
		}
	
	}

}
