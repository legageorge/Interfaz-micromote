// Soporte definitivo Interfaz Micromote
// dimension cilindro externo: diam=48.26 mm , alt=13 mm

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

aleta_x=diam_ext/12;
aleta_y=diam_ext/4;
aleta_z=alt_tot;

led_hole_diam=2.5;
led_hole_alt=2;

uusb_hole_x=8.8;
uusb_hole_y=5.6;
uusb_hole_z=3;

switch_hole_x=9.6;
switch_hole_y=7;
switch_hole_z=3.5;

tol=0.2;
tapa_hole_diam_inf=4;
tapa_hole_diam_sup=2.286;

union(){
	
	difference(){

		rotate_extrude ($fn=500) polygon (points=[[diam_ext/2,alt_tapa],[diam_ext/2,alt_tot],[0,alt_tot],[0,alt_tot-alt_sup],[diam_int/2,alt_tot-alt_sup],[diam_int/2,alt_tapa+alt_pilas+alt_pcb],[diam_pcb/2,alt_tapa+alt_pilas+alt_pcb],[diam_pcb/2,alt_tapa]]);

		translate([6.86,16.25,alt_tot]) cylinder(h=led_hole_alt, r=led_hole_diam/2, $fn=500, center=true);
		translate([-6.86,16.25,alt_tot]) cylinder(h=led_hole_alt, r=led_hole_diam/2, $fn=500, center=true);

		translate([0,-21.6,alt_tapa+alt_pilas+alt_pcb+uusb_hole_z/2]) cube([uusb_hole_x+tol,uusb_hole_y+tol,uusb_hole_z+tol], center=true);
		translate([0,20.57,alt_tapa+alt_pilas+alt_pcb+switch_hole_z/2]) cube([switch_hole_x+tol,switch_hole_y+tol,switch_hole_z+tol], center=true);

		translate([0,-diam_pcb/2,((alt_tapa+alt_pilas+alt_pcb)/2)]) cube([uusb_hole_x+tol,6,alt_tapa+alt_pilas+alt_pcb], center=true);
		translate([0,diam_pcb/2,((alt_tapa+alt_pilas+alt_pcb)/2)]) cube([switch_hole_x+tol,6,alt_tapa+alt_pilas+alt_pcb], center=true);

	}



	color("green"){
		union(){

			difference(){
				intersection(){
					difference(){
						translate([diam_ext/2,0,aleta_z/2]) {
							minkowski() {
								cube([aleta_x,aleta_y,aleta_z/2], center=true);
								cylinder(h=aleta_z/2, r=aleta_y/5, $fn=500, center=true);
							}
						}
						cylinder(h=alt_tot*2, r=diam_ext/2, $fn=500, center=true);
					}
				}
				translate([diam_ext/2+aleta_x/2,0,aleta_z/2]) cube([aleta_x/2,aleta_y*1.1,aleta_z*2], center=true);
			}

			mirror([1,0,0]){
				difference(){
					intersection(){
						difference(){
							translate([diam_ext/2,0,aleta_z/2]) {
								minkowski() {
									cube([aleta_x,aleta_y,aleta_z/2], center=true);
									cylinder(h=aleta_z/2, r=aleta_y/5, $fn=500, center=true);
								}
							}
							cylinder(h=alt_tot*2, r=diam_ext/2, $fn=500, center=true);
						}
					}
					translate([diam_ext/2+aleta_x/2,0,aleta_z/2]) cube([aleta_x/2,aleta_y*1.1,aleta_z*2], center=true);
				}

			}

		}
	}

}
