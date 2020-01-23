#ifndef __KLEUREN_H__
#define __KLEUREN_H__

#define MC                        100 // maximale kleur waarop de leds licht mogen geven
#define OFF                       (rgb_color){ 0  , 0  , 0   }
#define ON                        (rgb_color){ 255, 255, 255 }
#define WARM                      (rgb_color){ 255, 204, 32  }
#define TICKS_COLOR_VIJF          (rgb_color){ 5  , 0  , 0   }
#define TICKS_COLOR_VIJFTIEN      (rgb_color){ 5  , 5  , 5   }
#define TICKS_COLOR_VIJF_DIMM     (rgb_color){ 1  , 0  , 0   }
#define TICKS_COLOR_VIJFTIEN_DIMM (rgb_color){ 1  , 1  , 1   }

#define BLACK                     (rgb_color){ 0  , 0  , 0   }
#define RED                       (rgb_color){ MC , 0  , 0   }
#define GREEN                     (rgb_color){ 0  , MC , 0   }
#define BLUE                      (rgb_color){ 0  , 0  , MC  }
#define YELLOW                    (rgb_color){ MC , MC , 0   }
#define PURPLE                    (rgb_color){ MC , 0  , MC  } 
#define VIOLET                    (rgb_color){ 0  , MC , MC  } 
#define WHITE                     (rgb_color){ MC , MC , MC  }

#define TIKCKS_KLEUREN_AANTAL     7
rgb_color ticksKleur[TIKCKS_KLEUREN_AANTAL] = {RED, GREEN, BLUE, YELLOW, PURPLE, VIOLET, WHITE}; 

#endif



