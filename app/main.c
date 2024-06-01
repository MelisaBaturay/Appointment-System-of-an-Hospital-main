#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#define MAX_HASTA 40

typedef struct Hasta {
    int hastaNo;
    char hastaAdi[30];
    int hastaYasi;
    char cinsiyet[6];
    int mahkumlukDurumBilgisi;
    int engellilikOrani;
    char kanamaliHastaDurumBilgisi[15];
    char hastaKayitSaati[6];
    double muayeneSaati;
    int muayeneSuresi;
    int oncelikPuani;
} Hasta;

char saat[6];
const int hastaSayisi = 40;
char muayeneSaati[6]="09.00";
int muayenehastasayisi=0;
//Heap  veri yapýsýný ekleme
Hasta hastaListesi[MAX_HASTA];
Hasta siraliHastaListesi[MAX_HASTA];
Hasta kayitListesi[MAX_HASTA];

typedef struct Heap {
    Hasta* heapArray;
    int maxSize;
    int currentSize;
    int kayitSayisi;
} Heap;

void initializeHeap(Heap* heap, int maxSize) {
    heap->maxSize = maxSize;
    heap->currentSize = 0;
    heap->heapArray = (Hasta*)malloc(maxSize * sizeof(Hasta));
    heap->kayitSayisi=0;
}

void destroyHeap(Heap* heap) {
    free(heap->heapArray);
}

void insertToHeap(Heap* heap, Hasta item) {
    if (heap->currentSize == heap->maxSize) {
        printf("Daha fazla kayýt eklenemez.\n");
        return;
    }
	heap->kayitSayisi=heap->kayitSayisi+1;
    int currentIndex = heap->currentSize;
    heap->heapArray[currentIndex] = item;

    // Yeni eklenen elemanýn öncelik puanýný kontrol ederek heap'i yeniden düzenle
    while (currentIndex > 0 && heap->heapArray[currentIndex].oncelikPuani > heap->heapArray[(currentIndex - 1) / 2].oncelikPuani) {
        Hasta temp = heap->heapArray[currentIndex];
        heap->heapArray[currentIndex] = heap->heapArray[(currentIndex - 1) / 2];
        heap->heapArray[(currentIndex - 1) / 2] = temp;
        currentIndex = (currentIndex - 1) / 2;
    }

    heap->currentSize++;
}

Hasta removeFromHeap(Heap* heap) {
    if (heap->currentSize == 0) {
        printf("Heap boþ. herhangi bir item çýkartýlamaz .\n");
        Hasta emptyHasta = {0};
        return emptyHasta;
    }

    Hasta root = heap->heapArray[0];
    heap->currentSize--;
    heap->kayitSayisi=heap->kayitSayisi-1;

    heap->heapArray[0] = heap->heapArray[heap->currentSize];

    // Kök elemandan baþlayarak heap'i yeniden düzenle
    int currentIndex = 0;
    while (true) {
        int solCocuk = 2 * currentIndex + 1;
        int sagCocuk = 2 * currentIndex + 2;
        int enBuyuk = currentIndex;

        if (solCocuk < heap->currentSize && heap->heapArray[solCocuk].oncelikPuani > heap->heapArray[enBuyuk].oncelikPuani) {
            enBuyuk = solCocuk;
        }

        if (sagCocuk < heap->currentSize && heap->heapArray[sagCocuk].oncelikPuani > heap->heapArray[enBuyuk].oncelikPuani) {
            enBuyuk = sagCocuk;
        }

        if (enBuyuk != currentIndex) {
            Hasta temp = heap->heapArray[currentIndex];
            heap->heapArray[currentIndex] = heap->heapArray[enBuyuk];
            heap->heapArray[enBuyuk] = temp;
            currentIndex = enBuyuk;
        } 
		else
		 {
            break;
        }
    }

    return root;
}
///
void hastaKayitOku(Hasta* hastaListesi, int hastaSayisi, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya açýlamadý veya Dosya Boþ");
        return;
    }

    char satir[100];
    int index = 0;
    while (fgets(satir, sizeof(satir), dosya)) {
        char* token = strtok(satir, ",");

        hastaListesi[index].hastaNo = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[index].hastaAdi, token);
        token = strtok(NULL, ",");

        hastaListesi[index].hastaYasi = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[index].cinsiyet, token);
        token = strtok(NULL, ",");

        hastaListesi[index].mahkumlukDurumBilgisi = (strcmp(token, "true ") == 0) ? 1 : 0;
        token = strtok(NULL, ",");

        hastaListesi[index].engellilikOrani = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[index].kanamaliHastaDurumBilgisi, token);
        token = strtok(NULL, ",");
        
		strcpy(hastaListesi[index].hastaKayitSaati, token);
        token = strtok(NULL, ",");

        hastaListesi[index].muayeneSaati = atof(token);
        token = strtok(NULL, ",");

        hastaListesi[index].muayeneSuresi = atoi(token);
        token = strtok(NULL, ",");
		hastaListesi[index].oncelikPuani = oncelikPuaniHesapla(&hastaListesi[index]);
		hastaListesi[index].oncelikPuani=oncelikPuaniHesapla(hastaListesi[index]);
		hastaListesi[index].muayeneSuresi=muayeneSuresiHesapla(hastaListesi[index]);
        index++;     
    }

    fclose(dosya);
}

void hastaBilgileriniYazdir(const Hasta* hastaListesi, int hastaSayisi) {
    
	int i;
	for (i = 0; i < hastaSayisi; i++) {
        printf("Hasta No: %d\n", hastaListesi[i].hastaNo);
        printf("Hasta Adý: %s\n", hastaListesi[i].hastaAdi);
        printf("Hasta Yaþý: %d\n", hastaListesi[i].hastaYasi);
        printf("Cinsiyet: %s\n", hastaListesi[i].cinsiyet);
        printf("Mahkumluk Durumu: %s\n", hastaListesi[i].mahkumlukDurumBilgisi ? "Evet" : "Hayýr");
        printf("Engellilik Oraný: %d\n", hastaListesi[i].engellilikOrani);
        printf("Kanamalý Hasta Durumu: %s\n", hastaListesi[i].kanamaliHastaDurumBilgisi);
        printf("Hasta Kayýt Saati: %s\n", hastaListesi[i].hastaKayitSaati);
        printf("Muayene Saati: %.2f\n", hastaListesi[i].muayeneSaati);
        printf("Muayene Süresi: %d dakika\n", hastaListesi[i].muayeneSuresi);
        printf("Öncelik Puaný: %d\n", hastaListesi[i].oncelikPuani);
        printf("------------------------------------\n");
    }
}
////////////////////////////////////////////////////////////////////////////////////////
void hastaListesiniKayitSaatiSiralama(const Hasta* hastaListesi, int hastaSayisi, Hasta* siraliHastaListesi) {
    // Hasta listesini kayýt saatine göre sýralama
    int i;
    for (i = 0; i < hastaSayisi; i++) {
        siraliHastaListesi[i] = hastaListesi[i];
    }

    for (i = 0; i < hastaSayisi - 1; i++) {
    	int j;
        for (j = 0; j < hastaSayisi - i - 1; j++) {
            if (strcmp(siraliHastaListesi[j].hastaKayitSaati, siraliHastaListesi[j + 1].hastaKayitSaati) > 0) {
                Hasta temp = siraliHastaListesi[j];
                siraliHastaListesi[j] = siraliHastaListesi[j + 1];
                siraliHastaListesi[j + 1] = temp;
            }
        }
    }
    for (i = 0; i < hastaSayisi - 1; i++){
    	siraliHastaListesi[i].hastaNo=(i+1);
	}
}

int oncelikPuaniHesapla(const Hasta* hasta) {
    int oncelikPuani = 0;
    
    // Yaþa göre puan ekleme
    if (hasta->hastaYasi >= 0 && hasta->hastaYasi <= 5) {
        oncelikPuani += 20;
    } else if (hasta->hastaYasi > 5 && hasta->hastaYasi <= 45) {
        oncelikPuani += 0;
    } else if (hasta->hastaYasi > 45 && hasta->hastaYasi <= 65) {
        oncelikPuani += 15;
    } else if (hasta->hastaYasi > 65) {
        oncelikPuani += 25;
    }
    
    // Mahkumluk durumuna göre puan ekleme
    if (hasta->mahkumlukDurumBilgisi == 1) {
        oncelikPuani += 50;
    }
    
    // Engellilik oranýna göre puan ekleme
    oncelikPuani += (hasta->engellilikOrani) / 4;
    
    // Kanamalý hasta durumuna göre puan ekleme
    if (strcmp(hasta->kanamaliHastaDurumBilgisi, "kanama    ") == 0) {
        oncelikPuani += 20;
    } else if (strcmp(hasta->kanamaliHastaDurumBilgisi, "agirKanama") == 0) {
        oncelikPuani += 50;
    }
    
    return oncelikPuani;
}

int muayeneSuresiHesapla(const Hasta* hasta) {
    int muayeneSuresi = 10;  // Standart muayene süresi

    // Hasta yaþý 65'ten küçükse 0 dakika eklenir, aksi takdirde 15 dakika eklenir
    if (hasta->hastaYasi < 65) {
        muayeneSuresi += 0;
    } else {
        muayeneSuresi += 15;
    }

    // Engellilik oraný 5'e bölünerek dakika olarak eklenir
    muayeneSuresi += (hasta->engellilikOrani) / 5;

    // Kanamalý hasta durumu kontrol edilir ve buna göre dakika eklenir
    if (strcmp(hasta->kanamaliHastaDurumBilgisi, "kanama    ") == 0) {
        muayeneSuresi += 10;
    } else if (strcmp(hasta->kanamaliHastaDurumBilgisi, "agirKanama") == 0) {
        muayeneSuresi += 20;
    }

    return muayeneSuresi;
}

///hasta kayýt ekleme bölümü

void saatEkle(char *saat, int dakika) {
    int saat_saat, saat_dakika;
    sscanf(saat, "%d.%d", &saat_saat, &saat_dakika);

    saat_dakika += dakika;
    saat_saat += saat_dakika / 60;
    saat_saat=saat_saat%24;
    saat_dakika %= 60;

    sprintf(saat, "%02d.%02d", saat_saat, saat_dakika);
}

void HastaMuayeneEt(struct Hasta MuayeneHastasi) {
    printf("\n%d. sýrada %d numaralý %s adlý hasta muayene olmuþtur.\n", muayenehastasayisi, MuayeneHastasi.hastaNo, MuayeneHastasi.hastaAdi);
    printf("Öncelik Puaný: %d   Muayene süresi: %d  Muayene Giriþ Saati:%s\n", MuayeneHastasi.oncelikPuani, MuayeneHastasi.muayeneSuresi, muayeneSaati);
    saatEkle(muayeneSaati,MuayeneHastasi.muayeneSuresi);
}

int saatKarsilastir(char saat1[], char saat2[]) {
    int saat1_saat, saat1_dakika;
    int saat2_saat, saat2_dakika;

    sscanf(saat1, "%d.%d", &saat1_saat, &saat1_dakika);
    sscanf(saat2, "%d.%d", &saat2_saat, &saat2_dakika);

    if (saat1_saat > saat2_saat || (saat1_saat == saat2_saat && saat1_dakika > saat2_dakika)) {
        return 1;
    } else {
        return 0;
    }
}

void HastaKayitEkle() {
	Heap heap;
    initializeHeap(&heap, hastaSayisi);
	int i=0;

    while (i<hastaSayisi) {
    	if(heap.kayitSayisi == 0){
    		siraliHastaListesi[i].oncelikPuani = oncelikPuaniHesapla(&siraliHastaListesi[i]);
    		insertToHeap(&heap, siraliHastaListesi[i]);
    		strcpy(saat,siraliHastaListesi[i].hastaKayitSaati);
			i++;	
		}
		else if(saatKarsilastir(siraliHastaListesi[i].hastaKayitSaati,muayeneSaati)){
			muayenehastasayisi++;
			HastaMuayeneEt(removeFromHeap(&heap));
			
		}
		else{
			insertToHeap(&heap, siraliHastaListesi[i]);
			strcpy(saat,hastaListesi[i].hastaKayitSaati);
        	i++;
		}   
    }
    while(heap.kayitSayisi != 0){
    	muayenehastasayisi++;
        HastaMuayeneEt(removeFromHeap(&heap));
       	
	}
    destroyHeap(&heap);
	printf("\n\n TÜM HASTALAR BAÞARIYLA TEDAVÝ EDÝLDÝ...:)");
}

int main() {
    setlocale(LC_ALL, "Turkish");
    const char* dosyaAdi = "hasta.txt";


    hastaKayitOku(hastaListesi, hastaSayisi, dosyaAdi);
    hastaListesiniKayitSaatiSiralama(hastaListesi,hastaSayisi,siraliHastaListesi);
	hastaBilgileriniYazdir(siraliHastaListesi, hastaSayisi);
	
	HastaKayitEkle();
	
    return 0;
}
