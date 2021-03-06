/* Ecore_Con MCAST Usage Example
 */

#include <Ecore.h>
#include <Ecore_Con.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

Ecore_Con_Server *svr;

typedef int (*Handler_Func) (void *data, int type, void *event);

int
server_data (void *data,
	     int ev_type,
	     Ecore_Con_Event_Client_Data *ev) {

  printf("Server sent data 0x%08x!", ev->client);
  printf(":  %d %s\n", ev->size, (char *)(ev->data));
  return 1;
}

int
client_data (void *data,
	     int ev_type,
	     Ecore_Con_Event_Client_Data *ev) {

  printf("Client sent data 0x%08x!", ev->client);
  printf(":  %d %s\n", ev->size, (char *)(ev->data));
  return 1;
}

int
server_add (void *data,
	    int ev_type,
	    Ecore_Con_Event_Client_Add *ev) {
  printf("Server added\n");
  return 1;
}

int
server_del (void *data,
	    int ev_type,
	    Ecore_Con_Event_Client_Add *ev) {
  printf("Server deleted\n");
  exit(1);
}

int
client_add (void *data,
	    int ev_type,
	    Ecore_Con_Event_Client_Add *ev) {
  printf("Client connected\n");
  return 1;
}

int
client_del (void *data,
	    int ev_type,
	    Ecore_Con_Event_Client_Del *ev) {
  printf("Client disconnected\n");
  return 1;
}

Eina_Bool event_hup(void *data, int ev_type, void *ev)
{
  printf("Hup signal! Remove the server.\n");
  ecore_con_server_del(svr);
  return EINA_TRUE;
}

int main (int argc, char *argv[]) {
  const char msg[] = "Hello Server\n";

  if (argc != 3)
    {
      fprintf(stderr, "Usage: con_tcp_client_example host port\n");
      return 1;
    }

  ecore_con_init();

  ecore_event_handler_add(ECORE_EVENT_SIGNAL_HUP, 
                          event_hup, NULL);

  svr = ecore_con_server_connect(ECORE_CON_REMOTE_TCP | ECORE_CON_USE_SSL3, argv[1], atoi(argv[2]), NULL);
  if( !svr )
    {
      printf("Unable to connect\n");
      return 1;
    }
  else
    {
      printf("Trying to connect\n");
    }

  printf("Server handle: 0x%08x\n", svr);

  ecore_event_handler_add(ECORE_CON_EVENT_SERVER_ADD,
			  (Handler_Func)server_add, NULL);
  ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL,
			  (Handler_Func)server_del, NULL);
  ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD,
			  (Handler_Func)client_add, NULL);
  ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL,
			  (Handler_Func)client_del, NULL);
  ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA,
			  (Handler_Func)server_data, NULL);
  ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA,
			  (Handler_Func)client_data, NULL);

  ecore_con_server_send(svr, msg, sizeof(msg));

  ecore_main_loop_begin();

  ecore_con_shutdown();
  return 0;
}
