//========================================
//====Author: Vlad Zubets=================
//====E-mail: uladzislau.zubets@ua.pt=====
//========================================
#include <iostream>
#include <string>
#include <limits>
#include <complex> // numero complexo
#include <stdio.h> // *printf*
#include <math.h> // *exp*
#include <algorithm> //std::min_element, std::max_element
#include <fstream> // isso e para imprimir e gravar no ficheiro txt
using namespace std;
using std::cout; // cout--> consoule output de informacao
using std::endl; // end line
//===========================================================================================================
//===========================================================================================================
int main()
{
  
  double Pi = 3.98; // valor de entrada em mW 
  double alpha = 0.25; // valor de perdas na fibra 
  double t = 200e-12; // valor da largura de pulso de entrada em segundos (largura a meia altura)
  double dt = 1e-12; // intervalo do periodo
  double Area = 6.756e-5; // valor da area efetiva do nucleo em m^2
  double C = 0; // valor de chirp factor
  double Lambda = 1550e-9; // valor C.D.O. em metros
  double D = -10; // valor d coeficiente de dispersao em ps/nm/km D=-/2*pi*c*B2)/(lambda^2)
  double s = 100; // comprimento de fibra em Km
  const double c = 3e8; // velocidade de luz
  double n1 = 1.48; // indice de refracao de revestimento
  double n2 = 7e-10; // indice de refracao de nucleo
  const double pi = 3.1415926535897932384; // valor de pi
  complex < double > z( 0.0, 1.0 );     // z = 0 + 1i definicao do numero complexo
  double h = 2000; // passo h
//==============================================================================================================
//==============================================================================================================
  double A = alpha/(4.343); //vem da referencia
  
  double f = c/Lambda; // calcula a frequencia da onda EM
  
  double omega = 2*pi*f; // frequencia angular (w)
  
  double a = n2*omega; // primeira parte para calcular gamma
  
  double b = c*Area; // segunda parte para calcular gamma
  
  double gamma = a/b; // calcula gamma, ou seja, coeficiente nao-linear
  
  double B2 = -((pow(Lambda,2))*D)/(2*pi*c); // calcula o coeficiente de dispersao de velocidade de grupo (B2<0-->dispersao anomala, B2>0-->dispersao normal)
  
  double L = (pow(t,2))/(abs(B2)); // comprimento de dispersao
  
  double Ao = sqrt(Pi); // amplitude do impulso de entrada de Gauss
//=========================================================================================================================
//=========================================================================================================================
  ofstream myfile;
  myfile.open ("feixe_de_entrada.txt");
  
  int l = 8192; // e dimensao do vetor At
  
  double maxAt = std::numeric_limits<float>::min(); // para encontrar valor maximo de At
  
  double At[l] = {0}; // indice inicial de vetor At 
  
  double abs_At[l] = {0}; // indice inicial de abs de At
  
  unsigned int i = 0;
  
  for (double tau = -4096e-12; tau < 4095e-12; tau+=1e-12) // Qual é o tempo upper(U), lower(L) e intervalo entre intervalo upper e  lower intervalo(I)
  { 
  
      i++;
  
      At[i] = Ao*exp(-((1+C)/2)*(tau/t)*(tau/t));
      //complex< double > At; //ativar esta linha só se C for diferente de zero
      //double At = Ao*exp(-((1+z*C)/2)*(tau/t)*(tau/t)); // geracao do impulso de entrada de Gauss
  
      if(maxAt < At[i]) // para encontrar valor maximo de At
      {
  
            maxAt = At[i]; // para encontrar valor maximo de At
      
	  }       
  
      myfile << tau; myfile << ","; myfile << fabs(At[i]); myfile << "\n"; // para imprimir e gravar no ficheiro txt
  
      abs_At[i] = fabs(At[i]); // abs de At   
  
  }
  
  double abs_max_At_2 = fabs(maxAt/2); // escalar de abs(max(At)/2)
  //cout << abs_max_At_2 << endl; 
  
  unsigned int peaksi[l]; // indice inicial de pico acima de max(At)/2
  
  unsigned int npeaksi_fwhmi = 0; // indice inicial de numero de pico acima de max(At)/2
  
  for(i = 0; i < l; i++) // conta cada pico de At
  {
  
  	 if(abs_At[i] > abs_max_At_2) // para encontrar picos acima abs(max(At)/2)
	   {
  
  	   peaksi[npeaksi_fwhmi] = i; 
  
  	   npeaksi_fwhmi++; // dimensão do valor de fwhmi que e 471 
 	   //printf("npeaks_fwhmi=%d peaksi=%d\n",npeaksi_fwhmi,i);

	   }	

  }
  
  double dw = (1.0/l)/(dt*2*pi); // calcula a frequencia instantanea
  //cout << dw << endl;
 
//=============================================================================================================
//=========================FFT / IFFT==========================================================================
//=============================================================================================================
/*typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;

void fft(CArray& x) // Cooley–Tukey FFT (in-place)
{
    
	const size_t N = x.size();    
    if (N <= 1) return;
  
    CArray even = x[std::slice(0, N/2, 2)]; // divide
    CArray  odd = x[std::slice(1, N/2, 2)]; // divide
  
    fft(even); // conquer
    fft(odd); // conquer
  
    for (size_t k = 0; k < N/2; ++k) // combine
    {
        
        Complex t = std::polar(1.0f, -2 * c_pi * k / N) * odd[k]; // combine
        
        x[k    ] = even[k] + t; // combine
        
        x[k+N/2] = even[k] - t; // combine
    
    }

}

void ifft(CArray& x) // inverse fft (in-place)
{
	
    x = x.apply(std::conj); // conjugate the complex numbers
  
    fft( x ); // forward fft

    x = x.apply(std::conj); // conjugate the complex numbers again
  
    x /= (float)x.size(); // scale the numbers

}*/
//=============================================================================================================
//=============================================================================================================
  
  //At = fftshift(At);
  //spectrum = fft(fftshift(At));
  
  for (double w = (-1*l/2)*dw ; w < ((l/2)-1)*dw; w+=1*dw)
  {
  	
        //w = fftshift(w); // rearranja as saidas de fft movendo componentes de frequencias a zero ao centro do espetro
        
        for (double i = 0.1 ; i < (s/10); i+=0.1) // diferentes comprimentos de fibra
        {
        	
            double X = i*L;
            
            for (double j = h ; j < h; j+=X)
            {
                  //spectrum = spectrum*exp(-A*(h/2)+z*B2/2*w*w*(h));
                  //f = ifft(spectrum);
                  //f = f*exp(z*(8/9)*gamma*((abs(f))*(abs(f))*(h));
                  //spectrum = fft(f);
                  //spectrum = spectrum*exp(-A*(h/2)+i*B2/2*(w*w)*(h));
            }
            //f = ifft(spectrum);
            //output_pulse(q,:) = abs(f);
            //fwhmo = find(abs(f) > abs(max(f)/2));
            //fwhmo = length(fwhmo);
            //pbratio(q) = fwhmo/fwhmi;
            //q = q+1;
        }

  }
      
  /*for(int i = 0; i < sizeof(at)/8; i++){
      printf("%.100f\n",at[i]);
  } */
    //printf("%d\n",l);
    //printf("%100f\n",maxAt);
    
  myfile.close(); // fecha o ficheiro de gravação de At e tau  
  
  system("pause");
  
  return 0; // conclusao satisfatoria

}
