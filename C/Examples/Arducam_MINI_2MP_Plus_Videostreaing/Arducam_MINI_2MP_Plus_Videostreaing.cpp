#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "vga_graphics.h"
#include "ArduCAM.h"
#include "hardware/irq.h"
#include "ov2640_regs.h"
#include <cstdlib>
#include "stdio.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/mutex.h"

// import uart to read bmp from matlab
#include "hardware/uart.h"

// import comparison list of license plates
#include "plates.h"
#include "layer1.h"

#define UART_ID_1 uart1
#define BAUD_RATE 115200
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART1_TX_PIN 8
#define UART1_RX_PIN 9
int uart_counter = 0;

// include VGA drawing files 
// #include "../vga_graphics/vga_graphics.h"

#include "pt_cornell_rp2040_v1.h"
#include "pico/multicore.h"

// character array
char screentext[40];

void checkPlate(int* prediction, int num_digits); 

// uart command 

static mutex_t usb_mutex;
void SerialUsb(uint8_t* buffer,uint32_t lenght);
int SerialUSBAvailable(void);
int SerialUsbRead(void);
#define BMPIMAGEOFFSET 66
uint8_t bmp_header[BMPIMAGEOFFSET] =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};

// 20 x 20 array
#define NUM_DIGITS 2
float input_data[MAXSIZE] = { 0 };
// int prediction[2];
// digit values for vga draw (declare however many digits we are trying to print)
static volatile int d1;
static volatile int d2; 

// set pin 10 as the slave select for the digital pot:
#define LED_PIN 15
#define textX 10
const uint8_t CS = 5;
bool is_header = false;
int mode = 0;
uint8_t start_capture = 0;
// ArduCAM myCAM( OV2640, CS );
// uint8_t read_fifo_burst(ArduCAM myCAM);

// bool read_digit_flag = false;
static volatile bool plate_flag = false;


// Thread that draws to VGA display
static PT_THREAD (protothread_vga(struct pt *pt))
{
    // Indicate start of thread
    

    static char freqtext[40];
    PT_BEGIN(pt) ;
    // Write some text to VGA
    setTextColor(WHITE) ;


    // // Draw the static aspects of the display
    setCursor(65, 0);
    setTextSize(1);
    writeString("Final Project");
    setCursor(65, 10);
    setCursor(65, 20);
    writeString("Brian Hsu, Mariko McMurtry, Yingjie Zhao");
    setCursor(65, 30);
    writeString("HH543, MKM249, YZ483") ;
    int offset_x = 190;
    int offset_y = 100;
    int scl = 6;
    int dig_space = 140;
    // printf("\n enter vga pt_thread\n");
    // int count = 0;
    while (true) {

        //printf("p1 is %d\n", p1);
        // p3 = input_data[1202];
        // p4 = input_data[1203];
        
        // Wait on semaphore
        // count++;
        // printf("\n %d \n", count);
        // TODO: draw memory on VGA 
        // void drawPixel(short x, short y, char color)


      // TODO: Draw out input array 
      // if(read_digit_flag) {
        for(int i = 0; i < 20; i++) {
          for(int j = 0; j < 20; j++)
          {
            char color = input_data[i*20+j] == 1 ? WHITE : RED;
            fillRect((i+1)*scl + offset_x, (j+1)*scl + offset_y, scl, scl, color );
            // drawPixel((i+1)*10, )
          }
        }
        for(int i = 0; i < 20; i++) {
          for(int j = 0; j < 20; j++)
          {
            char color = input_data[i*20+j+400] == 1 ? WHITE : RED;
            fillRect((i+1)*scl + offset_x + dig_space, (j+1)*scl + offset_y, scl, scl, color );
            // drawPixel((i+1)*10, )
          }
        }
        // for(int i = 0; i < 20; i++) {
        //   for(int j = 0; j < 20; j++)
        //   {
        //     char color = input_data[i*20+j+800] == 1 ? WHITE : BLACK;
        //     fillRect((i+1)*10 + offset_x, (j+1)*10 + offset_y, 8, 8, color );
        //     // drawPixel((i+1)*10, )
        //   }
        // }
        // for(int i = 0; i < 20; i++) {
        //   for(int j = 0; j < 20; j++)
        //   {
        //     char color = input_data[i*20+j+1200] == 1 ? WHITE : BLACK;
        //     fillRect((i+1)*10 + offset_x, (j+1)*10 + offset_y, 8, 8, color );
        //     // drawPixel((i+1)*10, )
        //   }
        // }

        //TODO: add plate finder
        // char p1 = (char)prediction[0];
        // char p2 = (char)prediction[1];
        // int num1 = p1;
        // int num2 = p2;
        // // int num3 = p3;
        // // int num4 = p4;
        // int first_num;
        // int second_num;
        // int third_num;
        // int fouth_num;

        // for(int k = 0; k < 10; k++)
        // {
        //   int lnum = plate_2d_10[k];
        //   first_num = lnum % 10;
        //   second_num = (lnum / 10) % 10;
        //   // third_num = (lnum / 100) % 10;
        //   // fouth_num = lnum / 1000;
        //   // if(first_num == num4 && second_num == num3 && third_num == num2 && fouth_num == num1)
        //   if(first_num == num2 && second_num == num1)
        //   {
        //       plate_flag = true;
        //       break;
        //   }
        //   else 
        //       plate_flag = false;
        // }


        // read_digit_flag = false;
      // }
      char digit_predict;
      unsigned char digit_size;

      //TODO: draw arrow
      drawLine(300, 290, 325, 330, WHITE);
      drawLine(325, 330, 350, 290, WHITE);
      drawLine(325, 230, 325, 330, WHITE);

      // TODO: draw first predicted digit
      digit_predict = 48 + d2;
      // char digit_color = WHITE;
      // char digit_background = BLACK;
      digit_size = 8;
      setCursor(275, 350);
      setTextColor2(WHITE, BLACK);
      setTextSize(digit_size);
      tft_write(digit_predict);

      // // draw second predicted digit
      digit_predict = 48 + d1;
      setCursor(330, 350);
      setTextColor2(WHITE, BLACK);
      setTextSize(digit_size);
      tft_write(digit_predict);

      // drawPixel()
      PT_YIELD(pt);

    }
    // Indicate end of thread
    PT_END(pt);
}

static PT_THREAD (protothread_serial(struct pt *pt))
{
    PT_BEGIN(pt) ;
    static char classifier ;
    static int test_in ;
    static float float_in ;
    printf("\n enter serial pt_thread\n");
    while(1) {
        // serial_read;
        // sscanf(pt_serial_in_buffer, )
        sprintf(pt_serial_out_buffer, "\nEnter c to capture image: ");
        serial_write ;
        // spawn a thread to do the non-blocking serial read
        serial_read ;
        // convert input string to number
        sscanf(pt_serial_in_buffer,"%c", &classifier) ;

        // num_independents = test_in ;
        if (classifier=='c') {
          // sprintf(pt_serial_out_buffer, "")
          printf("counter = %d\n", uart_counter);
          uart_counter = 0;
            // TODO: camera commands for capture 
          // mode = 1;
          // usart_Command = 0xff;
          // start_capture = 1;
          // printf("ACK CMD CAM start single shoot. END");
        } 
        else {
          sprintf(pt_serial_out_buffer, "Invalid Command; Try again. \n");
          serial_write;
        }
        classifier = 0;
    }
    PT_END(pt) ;
}

static PT_THREAD (protothread_blink(struct pt *pt))
{
    // Indicate beginning of thread
    PT_BEGIN(pt) ;
    while (1) {
        // light up LED if plate is found
        if(plate_flag == true)
          gpio_put(LED_PIN, 1);
        else
          gpio_put(LED_PIN, 0);
        PT_YIELD_usec(500000) ;
    }
    PT_END(pt) ;
}

// Entry point for core 1
void core1_entry() {

    // pt_add_thread(protothread_serial);
    pt_add_thread(protothread_vga);
    pt_add_thread(protothread_blink);
    // pt_add_thread(protothread_vga);
    pt_schedule_start ;
}

int main() 
{
  // Initialize VGA
  initVGA() ;
  stdio_init_all();

  // Initialize LED  
  gpio_init(LED_PIN) ;
  gpio_set_dir(LED_PIN, GPIO_OUT) ;
  gpio_put(LED_PIN, 0) ;

  // init UART
  stdio_uart_init_full(uart1, BAUD_RATE, 8, 9);
  // uart_init(UART_ID_1, BAUD_RATE);

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  // gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
  // gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);

  int value=0;
  uint8_t vid, pid;
  uint8_t cameraCommand;
  // stdio_init_all();


  // start core 1 
  multicore_reset_core1();
  multicore_launch_core1(core1_entry);

  // start core 0
  // pt_add_thread(protothread_serial) ;
  // pt_add_thread(camerathread); // TODO: 
  // pt_schedule_start ;
  
  // configure uart 

  int prediction[NUM_DIGITS];


  while(1) {
    while(uart_is_readable(UART_ID_1)) {
      uint8_t value = uart_getc(UART_ID_1);
      input_data[uart_counter] = (float)value;
      uart_counter++; 
      //drawChar(50, 50, value, 3, 4, 11);
      // store in array


          
    }
    if(uart_counter == MAXSIZE) {
      printf("enter feedforward\n");
      // run predict function 
      // feedforward(float input[MAXSIZE], int SAMPLES, int* prediction)
      feedforward(input_data, NUM_DIGITS, prediction);
      // assign prediction array to the global digits for drawing
      d1 = prediction[1];
      d2 = prediction[0];
      printf("digit 1 is %d\n", d2);
      printf("digit 2 is %d\n", d1);
      checkPlate(prediction, NUM_DIGITS);

      //  linear()
      // activation()
      // linear() 
      // activation
      // predDgit
      // read_digit_flag = true;
      uart_counter = 0; 
    }
    
    // printf("counter %d\n", counter);
    // while(1){};
    // display on vga ?? 
    
  }
}

void checkPlate(int* prediction, int num_digits) {
  for(int k = 0; k < 10; k++)
  {
    int lnum = plate_2d_10[k];
    int first_num = lnum % 10;
    int second_num = (lnum / 10) % 10;
    int num1 = prediction[0];
    int num2 = prediction[1];
    // third_num = (lnum / 100) % 10;
    // fouth_num = lnum / 1000;
    // if(first_num == num4 && second_num == num3 && third_num == num2 && fouth_num == num1)
    if(first_num == num2 && second_num == num1)
    {
        plate_flag = true;
        printf("%d%d is a plate", second_num, first_num);
        break;
    }
    else 
        plate_flag = false;
  }
}

void SerialUsb(uint8_t* buf,uint32_t length)
{
    static uint64_t last_avail_time;
    int i = 0;
    if (tud_cdc_connected()) 
    {
        for (int i = 0; i < length;) 
        {
            int n = length - i;
            int avail = tud_cdc_write_available();
            if (n > avail) n = avail;
            if (n) 
            {
                int n2 = tud_cdc_write(buf + i, n);
                tud_task();
                tud_cdc_write_flush();
                i += n2;
                last_avail_time = time_us_64();
            } 
            else 
            {
                tud_task();
                tud_cdc_write_flush();
                if (!tud_cdc_connected() ||
                    (!tud_cdc_write_available() && time_us_64() > last_avail_time + 1000000 /* 1 second */)) {
                    break;
                }
            }
        }
    } 
    else 
    {
        // reset our timeout
        last_avail_time = 0;
    }
}

int SerialUSBAvailable(void)
{
  return tud_cdc_available();
} 

int SerialUsbRead(void) 
{
  if (tud_cdc_connected() && tud_cdc_available()) 
  {
    return tud_cdc_read_char();
  }
  return -1;
}

