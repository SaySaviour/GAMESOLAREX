#include <stdio.h>
#include "allegro-5.0.10-mingw-4.7.0\include\allegro5\allegro.h"
#include "allegro-5.0.10-mingw-4.7.0\include\allegro5\allegro_image.h"
#include "allegro-5.0.10-mingw-4.7.0\include\allegro5\allegro_primitives.h"
#include "allegro-5.0.10-mingw-4.7.0\include\allegro5\allegro_acodec.h"
#include "allegro-5.0.10-mingw-4.7.0\include\allegro5\allegro_audio.h"
const float FPS = 60;
const int ekran_en=800;
const int ekran_boy=600;
const int oyuncu_boyutu=32;
enum HareketAnahtarlari{KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT};
int main(int argc, char *argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *oyuncu=NULL;
	float oyuncu_x=ekran_boy/ 2.0-oyuncu_boyutu/ 2.0;
	float oyuncu_y=ekran_en/ 2.0-oyuncu_boyutu/ 2.0;
    bool  key[4]={false,false,false,false};
	bool redraw = true;
	bool cikisyap=false;
    
	// Allegro Yüklemesi
	if (!al_init()) {
		fprintf(stderr, "Allegro kutuphanesi Yuklenemedi\n");
		return -1;
	}
	if (!al_install_keyboard())
	{
		fprintf(stderr,"Klavye Baslatılamadi\n");
		return -1;
	}
	
   al_init_image_addon();
	// Tİmer Oluşturma
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Zaman Olusturulamadi\n");
		return -1;
	}

	// Ekran Oluşturma (En ve Boyu)
	display = al_create_display(ekran_en, ekran_boy);
	if (!display) {
		fprintf(stderr, "Ekran Olusturulamadi\n");
		al_destroy_timer(timer);
		return -1;
	}
	//Resim Olarak Oluşturduğum Oyuncuyu Bitmap halinde oyun içine aktarıyorum eğer yüklenmezse oyuncu yüklenmedi.....
    oyuncu=al_load_bitmap("Oyuncu.png");
	if (!oyuncu)
	{
		fprintf(stderr,"Oyuncu Yuklenemedi");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	al_set_target_bitmap(oyuncu);
	al_set_target_bitmap(al_get_backbuffer(display));
	
	// Olay Sıralarının Oluşturulması....
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		al_destroy_bitmap(oyuncu);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	// Etkinlikleri Kaydetme
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue,al_get_keyboard_event_source());
	// Arka Plan oluşturma
	al_clear_to_color(al_map_rgb(170, 255, 0));
	al_flip_display();
	// Timer Başlatma
	al_start_timer(timer);

	// Oyun Çalışırken
	while (!cikisyap) {
		ALLEGRO_EVENT event;
		
		 al_wait_for_event(event_queue, &event);

		// Tuş Atamaları
		if (event.type==ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP] && oyuncu_y>=4.0)
			{
				oyuncu_y-=4.0;
			}
			if (key[KEY_DOWN] && oyuncu_y<=ekran_boy-oyuncu_boyutu-4.0)
			{
				oyuncu_y+=4.0;
			}
			if (key[KEY_LEFT] && oyuncu_x>=4.0)
			{
				oyuncu_x-=4.0;
			}
			if (key[KEY_RIGHT] && oyuncu_x<=ekran_en-oyuncu_boyutu-4.0)
			{
				oyuncu_x+=4.0;
			}
			redraw=true;
		}
		else if (event.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}
		else if (event.type==ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				key[KEY_UP]=true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN]=true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT]=true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT]=true;
				break;
			}
		}
		else if (event.type==ALLEGRO_EVENT_KEY_UP)
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				key[KEY_UP]=false;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN]=false;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT]=false;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT]=false;
				break;
		    case ALLEGRO_KEY_ESCAPE:
			cikisyap=true;
			break;
			}
		}
		
		if (redraw && al_is_event_queue_empty(event_queue)) {
		
			al_clear_to_color(al_map_rgb(170, 255, 0));
	        al_draw_bitmap(oyuncu,oyuncu_x,oyuncu_y,0);
			al_flip_display();
			
			redraw = false;
		}
		
	} 

	// Tüm işlemler bittiğinde kapatması için ...
	al_destroy_display(display);
	al_destroy_bitmap(oyuncu);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	return 0;
}