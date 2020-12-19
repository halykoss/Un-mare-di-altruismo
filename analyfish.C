
void simul(){
                                                // CAMPO, EPOCA, NUM PESC, NUM CIBO,         ALTRUISMO, VELOCITÀ, ENERGIA, ETÀ, TRIGGER


int num_righe = 1000; //quante righe ha il file csv IMPORTANTE

int *fish_num = new int [num_righe];
double *kindness = new double [num_righe];
double *speed = new double [num_righe];

for (int i=0; i<num_righe; i++) {
 //numero pesci
	fish_num[i] = get_int(i,2);
	kindness[i] = 0;
	speed[i] = 0;
 //calcolo altruismo medio e velocità media
	for (int j=0; j<fish_num[i]; j++) {
		kindness[i] += get_int(i,4+5*j);
		speed[i] += get_int(i,5+5*j)
	}
	kindness[i] /= fish_num[i];
	speed[i] /= fish_num[i];
}


int n_simul = 5;

double en_comb[n_simul] = {0,0,0,0,0};      //da riempire a mano
double l_kindness[n_simul] = {0,0,0,0,0};   //da riempire a mano
double perc_banchi[n_simul] = {0,0,0,0,0};  //da riempire a mano


----------------------------------------------------------------// grafico energia combattimento e altruismo delle ultime n epoche (più simulazioni)

auto *graph = new TGraph (n_simul, en_comb, l_kindness);
graph->SetTitle("Energia di combattimento e altruismo");
graph->GetXaxis()->SetTitle("Energia di combattimento");
graph->GetYaxis()->SetTitle("Altruismo");
graph->Draw();

/*TF1 *funz = new TF1("funz", "[0]+[1]*x", 40,800);
funz->SetParameters(0,1);*/

------------------------------------------------------------------------// grafico percentuale banchi e altruismo delle ultime n epoche (più simulazioni)

auto *graph = new TGraph (n_simul, perc_banchi, l_kindness);
graph->SetTitle("Percentuale banchi e altruismo");
graph->GetXaxis()->SetTitle("Percentuale banchi");
graph->GetYaxis()->SetTitle("Altruismo");
graph->Draw();

-----------------------------------------------------------------------// grafico numero pesci e altruismo

auto *graph = new TGraph (num_righe, fish_num, kindness);
graph->SetTitle("Numero pesci e altruismo");
graph->GetXaxis()->SetTitle("Numero pesci");
graph->GetYaxis()->SetTitle("Altruismo");
graph->Draw();

----------------------------------------------------------------------// grafico velocità e altruismo

auto *graph = new TGraph (n, speed, kindness);
graph->SetTitle("Velocità e altruismo");
graph->GetXaxis()->SetTitle("Velocità");
graph->GetYaxis()->SetTitle("Altruismo");
graph->Draw();

------------------------------------------------------------------// istogramma altruismo medio sulle ultime n epoche per ogni campo

int last_n = 100;
int count[9] = {0,0,0,0,0,0,0,0,0};
double ls_kind[9] = {0,0,0,0,0,0,0,0,0};
double ls_fish[9] = {0,0,0,0,0,0,0,0,0};
bool search = true;
int a;

for (int y=0; y<9; y++) {
	search = true;
	for (int z=0; search; z++) {
		if (get_int(num_righe - z,0)==y) {
			a = get_int(num_righe - z,2);
			ls_fish[y] += a;
			for (int x=0; x<a; x++) {
				ls_kind[y] += get_int(num_righe - z,4+5*x);	
			}
			count[y]++;
			if (count[y]==last_n) {
				search = false;
			}
		}
	}

}


TH1F* h = new TH1F("h","altruismo medio",9,0,9);
for (i=0; i<9; i++){
	h->AddBinContent(ls_kind[i] / ls_fish[i]);
}



cout<<"Media altruismo =\t"<< (ls_kind[0]+ls_kind[1]+ls_kind[2]+ls_kind[3]+ls_kind[4]+ls_kind[5]+ls_kind[6]+ls_kind[7]+ls_kind[8])/(ls_fish[0]+ls_fish[1]+ls_fish[2]+ls_fish[3]+ls_fish[4]+ls_fish[5]+ls_fish[6]+ls_fish[7]+ls_fish[8]);

}

