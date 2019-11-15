#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define N 50
#define N_SRC 4*N+1 
#define N_DST 4*N+1 


#define pas_clear_string(dst_str)          (dst_str)[0] = 0
#define pas_copy_string(dst_str, src_str)  strcpy ((dst_str), (src_str))
#define pas_get_string_length(str)         strlen ((str))
#define pas_get_string_char(str, n)        (str)[(n)]
#define pas_append_char_to_string(str, c)  { int l = strlen((str)); (str)[l] = (c); (str)[l+1]=0; }


#include "info.h"

void perror_and_exit(char *s, int exit_code)
{
	perror(s);
	exit(exit_code);
}



static int
char_to_int (char c)
{
  int res;

  switch (c)
    {
      case '0': res = 0; break;
      case '1': res = 1; break;
      case '2': res = 2; break;
      case '3': res = 3; break;
      case '4': res = 4; break;
      case '5': res = 5; break;
      case '6': res = 6; break;
      case '7': res = 7; break;
      case '8': res = 8; break;
      case '9': res = 9; break;
      case 'A':
      case 'a': res = 10; break;
      case 'B':
      case 'b': res = 11; break;
      case 'C':
      case 'c': res = 12; break;
      case 'D':
      case 'd': res = 13; break;
      case 'E':
      case 'e': res = 14; break;
      case 'F':
      case 'f': res = 15; break;
      default:
        res = -1; break;
    }

  return res;
}
static char
int_to_char (int n)
{
  char res;

  switch (n)
    {
      case 0: res = '0'; break;
      case 1: res = '1'; break;
      case 2: res = '2'; break;
      case 3: res = '3'; break;
      case 4: res = '4'; break;
      case 5: res = '5'; break;
      case 6: res = '6'; break;
      case 7: res = '7'; break;
      case 8: res = '8'; break;
      case 9: res = '9'; break;
      case 10: res = 'A'; break;
      case 11: res = 'B'; break;
      case 12: res = 'C'; break;
      case 13: res = 'D'; break;
      case 14: res = 'E'; break;
      case 15: res = 'F'; break;
      default:
        res = ""; break;
    }

  return res;
}

static void
divide_string (int radix,             
               char *divident_str,    
               int divisor,           
               char *res_quotient_str, 
               int *res_reminder_p)    
{
  int divident_len; 
  int divident_ind; 
  int cur_divident; 
  int cur_quotient; 
  int cur_reminder; /* òåêóùèé îñòàòîê */
  int int_val;      /* ïðîìåæóòî÷íîå çíà÷åíèå äëÿ öåëîãî */
  char char_val;    /* ïðîìåæóòî÷íîå çíà÷åíèå äëÿ ñèìâîëà */
  int flag_first;

/*  printf ("=== START DIVIDE: radix=%d, divident_str=%s, divisor=%d\n", radix, divident_str, divisor); */

  /* Î÷èùàåì ñòðîêó äëÿ ÷àñòíîãî */
  pas_clear_string (res_quotient_str);

  /* Âû÷èñëÿåì äëèíó äåëèìîãî è óñòàíàâëèâàåì òåêóùèé ðàçðÿä
   * íà íà÷àëî ñòðîêîâãî ïðåäñòàâëåíèÿ */
  divident_len = pas_get_string_length (divident_str);
  divident_ind = 0;

  /* Òåêóùåå çíà÷åíèå äåëèìîãî (ïî ñóòè äëÿ îäíîé èòåðàöèè äåëåíèÿ ñòîëáèêîì) */
  cur_divident = 0;

  /* Öèêë ïî ðàçðÿäàì äåëèòåëÿ */
  do
    {
      /* Åñëè òåêóùåå çíà÷åíèå äåëèìîãî ìåíüøå äåëèòåëÿ è åñëè åñòü åù¸
       * ñâîáîäíûå ðàçðÿäû, òî ñïóñêàåì ñëåäóþùèé ðàçðÿä è ïðèêëåèâàåì
       * åãî ê òåêóùåìó çíà÷åíèþ äåëèìîãî */
      flag_first = 0;
      while (cur_divident < divisor && divident_ind < divident_len)
        {
          char_val = pas_get_string_char (divident_str, divident_ind);
          divident_ind++;
          int_val = char_to_int (char_val);
          if (int_val == -1)
            continue;
          cur_divident = cur_divident * radix + int_val;

          /* Åñëè çà ðàç ïðèõîäèòñÿ ñïóñêàòü áîëåå îäíîãî ðàçðÿäà, òî
           * íà êàæäûé ðàçðÿä, íà÷èíàÿ ñî âòîðîãî, íóæíî ê ðåçóëüòàòó
           * äîðèñîâûâàòü íîëèê. Åñëè â ÷àñòíîì ó íàñ åù¸ íè÷åãîíå çàïèñàíî,
           * òî ýòîãî äåëàòü íå íàäî. Òàêîå âîçíèêàåò èç-çà òîãî, ÷òî
           * ñàìû ñòàðøèé ðàçðÿä äåëèìîãî ìû ïî ñóòè äåëà òîæå ñïóñêàåì */
          if (flag_first == 1 && pas_get_string_length( res_quotient_str) > 0)
            {
              char_val = int_to_char (0);
              if (char_val = "")
                continue;
              pas_append_char_to_string (res_quotient_str, char_val);
            }

          flag_first = 1;
        }

      /* ×àñòíîå îò äåëåíèÿ - ýòî òî, ÷òî ïðè äåëåíèè ñòîëáèêîì çàïèñûâàåì
       * â î÷åðåäíîé ðàçðÿä ÷àñòíîãî, îñòàòîê - ýòî òî, ÷òî ïåðåíåñ¸ì
       * íà ñëåäóþùóþ èòåðàöèþ äåëåíèÿ (èëè èòîãîâûé îñòàòîê) */
      cur_quotient = cur_divident / divisor;
      cur_reminder = cur_divident % divisor;

      /* Äîïèñûâàåì òåêóùèé ðàçðÿä ÷àñòíîãî ê ñòðîêå */
      int_val = cur_quotient;
      char_val = int_to_char (int_val);
      pas_append_char_to_string (res_quotient_str, char_val);

      /* Îñòàòîê òåïåðü ñòàíîâèòñÿ äåëèìûì äëÿ ñëåäóþùåé èòåðàöèè */
      cur_divident = cur_reminder;
    }
  while (divident_ind < divident_len);

  /* Â öèêëå ìû óæå çàïèñàëè ðåçóëüòèðóþùåå ÷àñòíîå. Çàïèøåì ðåçóëüòèðóþùèé îñòàòîê */
  *res_reminder_p = cur_reminder;

/*  printf ("=== FINISH DIVIDE: res_quotient_str=%s, cur_reminder=%d\n", res_quotient_str, cur_reminder); */
}

/* Ïðîâåðêà ñòðîêîâîãî ïðåäñòàâëåíèÿ ÷èñëà íà íîëü. Ïîëàãàåì, ÷òî
 * êðèòåðèåì áóäåò äëèíà ñòðîêè åäèíèöà, è íóëåâîé ýëåìåíò ñòðîêè - 0 */
static int
is_number_zero (char *str)
{
  if (pas_get_string_length (str) == 1
      && pas_get_string_char (str, 0) == '0')
    return 1;

  return 0;
}

/* Ïðåîáðàçîâàíèå ñòðîêîâîãî âèäà ÷èñëà èç äåñÿòè÷íîé ñèñòåìû èñ÷èñëåíèÿ
 * â äâîè÷íóþ. Îòâåòñâåííîñòü çà ïðàâèëüíûå ñòðîêîâûå áóôôåðà âîçëàãàåì
 * íà âûçûâàþùóþ ïðîöåäóðó */
static void
conv_dec_to_bin (char *src_str,  /* âõîäíàÿ ñòðîêà */
                 int src_radix,  /* ñèñòåìà èñ÷èñëåíèÿ âõîäíîãî ÷èñëà */
                 char *res_str,  /* ðåçóëüòèðóþùàÿ ñòðîêà */
                 int res_radix)  /* ñèñòåìà èñ÷èñëåíèÿ ðåçóëüòèðóþùåãî ÷èñëà */
{
  char divident_str[N_SRC]; /* ñòðîêà äëÿ äåëèòåëÿ */
  char quotient_str[N_SRC]; /* ñòðîêà äëÿ ÷àñòíîãî */
  int reminder;             /* îñòàòîê */
  int digits[N_DST];        /* ìàññèâ äëÿ õðàíåíèÿ îñòàòêîâ */
  int digit_ind;            /* òåêóùàÿ ïîçèöèÿ â ìàññèâå digits */
  int int_val;              /* ïðîìåæóòî÷íîå çíà÷åíèå äëÿ öåëîãî */
  char char_val;            /* ïðîìåæóòî÷íîå çíà÷åíèå äëÿ ñèìâîëà */
  int i;

  digit_ind = 0;

  pas_copy_string (divident_str, src_str);

  do
    {
      divide_string (src_radix, divident_str, res_radix, quotient_str, &reminder);

      digits[digit_ind] = reminder;
      digit_ind++;

      pas_copy_string (divident_str, quotient_str);
    }
  while (! is_number_zero (quotient_str));

  pas_clear_string (res_str);
  for (i = digit_ind - 1; i >=0 ; i--)
    {
      int_val = digits[i];
      char_val = int_to_char (int_val);
      pas_append_char_to_string (res_str, char_val);
    }
}

int main()
{
    int sock, listener;
    struct sockaddr_in addr, client_addr;
    char buf[MSG_LEN];
    char old_buf[MSG_LEN];
    char new_buf[MSG_LEN];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
        perror_and_exit("socket()", 1);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SOCK_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror_and_exit("bind()", 2);

    listen(listener, 1);
    printf("Server is listening...\n");
    
    while(1)
    {
        socklen_t cli_addr_size = sizeof(client_addr);
        sock = accept(listener, (struct sockaddr*) &client_addr, &cli_addr_size);
        if(sock < 0)
            perror_and_exit("accept()", 3);


        printf("Received packet from %s:%d\n\n", 
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));


        while(1)
        {

            bytes_read = recv(sock, buf, MSG_LEN, 0);
            if(bytes_read <= 0) break;

            FILE *fp;
            char name[] = "ip_adresses.txt";
            if ((fp = fopen(name, "r")) == NULL)
            {
                return 0;
            }
            int index = atoi(buf);
            int i = 0;
            while (1)
            {

                char *estr;
                estr = fgets (buf,sizeof(buf),fp);
              //Проверка на конец файла или ошибку чтения
                if (estr == NULL)
                {
                     // Проверяем, что именно произошло: кончился файл
                     // или это ошибка чтения
                     if ( feof (fp) != 0)
                     {  
                        //Если файл закончился, выводим сообщение о завершении 
                        //чтения и выходим из бесконечного цикла
                        
                        break;
                     }
                     else
                     {
                        //Если при чтении произошла ошибка, выводим сообщение 
                        //об ошибке и выходим из бесконечного цикла
                        
                        break;
                     }
                }
                //Если файл не закончился, и не было ошибки чтения 
                //выводим считанную строку  на экран
                if (i == index)
                {
                    break;
                }

                strcpy(old_buf, buf);
                i++;
            }
            send(sock, old_buf, sizeof(old_buf), 0);
            conv_dec_to_bin(old_buf, 10, new_buf, 16);
            printf("%s\n", new_buf);
            send(sock, new_buf, sizeof(new_buf), 0);
        }
    
        close(sock);
    }
    
    return 0;
}
