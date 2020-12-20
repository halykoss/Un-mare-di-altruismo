int get_int(int row, int cols){
  string line;
  ifstream myfile("values.csv");
  if (myfile.is_open())
  {
    for(;!myfile.eof();row--){
      getline (myfile,line);
     if (row == 0)
      {
      	std::string delimiter = ",";

	size_t pos = 0;
	int counter = 0;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos) {
    		token = line.substr(0, pos);
    		if(counter == cols){
    			return atoi(token.c_str());
    		}
    		line.erase(0, pos + delimiter.length());
    		counter++;
	}
    	if(counter == cols){
    		return atoi(line.c_str());
    	}
      }
    }
    myfile.close();	
  }
  return 0;
}

float get_float(int row, int cols){
  string line;
  ifstream myfile("values.csv");
  if (myfile.is_open())
  {
    for(;!myfile.eof();row--){
      getline (myfile,line);
     if (row == 0)
      {
      	std::string delimiter = ",";

	size_t pos = 0;
	int counter = 0;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos) {
    		token = line.substr(0, pos);
    		if(counter == cols){
    			return stof(token.c_str());
    		}
    		line.erase(0, pos + delimiter.length());
    		counter++;
	}
    	if(counter == cols){
    		return stof(line.c_str());
    	}
      }
    }
    myfile.close();	
  }
  return 0.0;
}



void simul(){
                                                // CAMPO, EPOCA, NUM PESC, NUM CIBO,         ALTRUISMO, VELOCITÀ, ENERGIA, ETÀ, TRIGGER


int num_righe = 9033; //quante righe ha il file csv IMPORTANTE

int *fish_num = new int [num_righe];
double *kindness = new double [num_righe];
double *speed = new double [num_righe];

for (int i=0; i<num_righe; i++) {
	if (i%100==0) {cout<< i << endl;}
 //numero pesci
	fish_num[i] = get_int(i,2);
	kindness[i] = 0;
	speed[i] = 0;
 //calcolo altruismo medio e velocità media
	for (int j=0; j<fish_num[i]; j++) {
		kindness[i] += get_int(i,4+5*j);
		speed[i] += get_int(i,5+5*j);
	}
	kindness[i] /= fish_num[i];
	speed[i] /= fish_num[i];
}

/*
int n_simul = 5;

double en_comb[n_simul] = {0,0,0,0,0};      //da riempire a mano
double l_kindness[n_simul] = {0,0,0,0,0};   //da riempire a mano
double perc_banchi[n_simul] = {0,0,0,0,0};  //da riempire a mano


//---------------------------------------------------------------- grafico energia combattimento e altruismo delle ultime n epoche (più simulazioni)
TCanvas * c3 = new TCanvas("c3","c3");

auto *graph3 = new TGraph (n_simul, en_comb, l_kindness);
graph3->SetTitle("Energia di combattimento e altruismo");
graph3->GetXaxis()->SetTitle("Energia di combattimento");
graph3->GetYaxis()->SetTitle("Altruismo");
graph3->Draw("AP");

//------------------------------------------------------------------------ grafico percentuale banchi e altruismo delle ultime n epoche (più simulazioni)
TCanvas * c4 = new TCanvas("c4","c4");

auto *graph4 = new TGraph (n_simul, perc_banchi, l_kindness);
graph4->SetTitle("Percentuale banchi e altruismo");
graph4->GetXaxis()->SetTitle("Percentuale banchi");
graph4->GetYaxis()->SetTitle("Altruismo");
graph4->Draw("AP");
*/
//----------------------------------------------------------------------- grafico numero pesci e altruismo
TCanvas * c1 = new TCanvas("c1","c1");

TGraph *graph1 = new TGraph (num_righe);
graph1->SetTitle("Numero pesci e altruismo");
graph1->GetXaxis()->SetTitle("Numero pesci");
graph1->GetYaxis()->SetTitle("Altruismo");
for (int i=0; i<num_righe; i++) {
	graph1->SetPoint(i,fish_num[i], kindness[i]);
}
graph1->Draw("AP");

//---------------------------------------------------------------------- grafico velocità e altruismo
TCanvas * c2 = new TCanvas("c2","c2");

TGraph *graph2 = new TGraph (num_righe);
graph2->SetTitle("Velocità e altruismo");
graph2->GetXaxis()->SetTitle("Velocità");
graph2->GetYaxis()->SetTitle("Altruismo");
for (int i=0; i<num_righe; i++) {
	graph2->SetPoint(i,speed[i], kindness[i]);
}
graph2->Draw("AP");

//------------------------------------------------------------------ istogramma altruismo medio sulle ultime n epoche per ogni campo


int last_n = 100;
int count[9] = {0,0,0,0,0,0,0,0,0};
double ls_kind[9] = {0,0,0,0,0,0,0,0,0};
double ls_fish[9] = {0,0,0,0,0,0,0,0,0};
bool search = true;
int a;

for (int y=0; y<9; y++) {
	cout<< y << endl;
	search = true;
	for (int z=1; search; z++) {
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

TCanvas * c5 = new TCanvas("c5","c5");

TH1F* h = new TH1F("h","altruismo medio",9,0,9);
for (int i=0; i<9; i++){
	h->AddBinContent(i+1,ls_kind[i] / ls_fish[i]);
}
h->Draw();

double sum1 = ls_kind[0]+ls_kind[1]+ls_kind[2]+ls_kind[3]+ls_kind[4]+ls_kind[5]+ls_kind[6]+ls_kind[7]+ls_kind[8];
double sum2 = ls_fish[0]+ls_fish[1]+ls_fish[2]+ls_fish[3]+ls_fish[4]+ls_fish[5]+ls_fish[6]+ls_fish[7]+ls_fish[8];
cout<<"Media altruismo =\t"<< sum1/sum2;

}

