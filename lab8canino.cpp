#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <math.h>

int main(int argc, char** argv) {
    const float FPS = 60;
    const int SCREEN_W = 640;
    const int SCREEN_H = 480;
    const int fish_SIZE = 32;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    float fish_x = SCREEN_W / 2.0 - fish_SIZE / 2.0;
    float fish_y = SCREEN_H / 2.0 - fish_SIZE / 2.0;
    float fish_dx = -4.0, duck_dy = 4.0;
    float angle = 0.0;
    bool redraw = true;
    bool pause = false; 
    int flags = 0;

    ALLEGRO_BITMAP* ocean = NULL;
    ALLEGRO_BITMAP* fish = NULL;

    if (!al_init()) {
        return -1;
    }
    if (!al_install_keyboard()) {
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        al_destroy_timer(timer);
        return -1;
    }

    al_init_image_addon();
    ocean = al_load_bitmap("ocean.png");
    fish = al_load_bitmap("clownfish.png");
    al_convert_mask_to_alpha(fish, al_map_rgb(255, 0, 255));

    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_bitmap(fish);
        al_destroy_bitmap(ocean);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); 

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);

    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (!pause) {
                bool wall_touch = false;

                if (fish_x < 0 || fish_x > SCREEN_W - fish_SIZE) {
                    fish_dx = -fish_dx;
                    angle = 0.0;
                    wall_touch = true; //boolean appears in these now to check collision
                }
                if (fish_y < 0 || fish_y > SCREEN_H - fish_SIZE) {
                    duck_dy = -duck_dy;
                    angle = 0.0;
                    wall_touch = true;
                }
                if (fish_dx == 0 && angle < 3) {
                    angle += .1;
                }
                if (fish_dx == (SCREEN_W - fish_SIZE) && angle >= 0) {
                    angle -= .1;
                }
                if (angle < 3.14) {
                    angle += 0.1; //pulled from Canvas assignment description
                }
                fish_x += fish_dx;
                fish_y += duck_dy;
            }
            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                pause = !pause; 
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                flags |= ALLEGRO_FLIP_HORIZONTAL;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                flags &= ~ALLEGRO_FLIP_HORIZONTAL;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                flags |= ALLEGRO_FLIP_VERTICAL;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                flags &= ~ALLEGRO_FLIP_VERTICAL;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (ocean) {
                al_draw_bitmap(ocean, 0, 0, 0);
            }
            if (fish) {
                al_draw_rotated_bitmap(fish, 16, 16, fish_x + 16, fish_y + 16, angle, flags);
            }
            al_flip_display();
        }
    }

    al_destroy_bitmap(fish);
    al_destroy_bitmap(ocean);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    system("pause");
    return 0;
}
