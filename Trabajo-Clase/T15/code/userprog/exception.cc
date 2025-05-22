// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "nachostabla.h"
#include <fcntl.h>  // Al inicio del archivo
#include <unistd.h> // Para close()

//Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void returnFromSystemCall(); // forward declaration
void Nachos_ForkThread(void *arg); // forward declaration

// Estructura para pasar argumentos a NachosForkThread
struct ForkArgs
{
   void *userFunc;
   int stackBase;
};

/*
 *  System call interface: Halt()
 */
void NachOS_Halt() {		// System call 0

	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();

}


/*
 *  System call interface: void Exit( int )
 */
void NachOS_Exit() {		// System call 1
   // Avanzar el PC para evitar repetir el syscall
   returnFromSystemCall();
   int status = machine->ReadRegister(4); // obtener el status de salida
   DEBUG('u', "Exit system call invoked with status %d\n", status);
   currentThread->Finish(); // finaliza el hilo actual
}

/*
 *  System call interface: SpaceId Exec( char * )
 */
void NachOS_Exec() {		// System call 2
}


/*
 *  System call interface: int Join( SpaceId )
 */
void NachOS_Join() {		// System call 3
}


/*
 *  System call interface: void Create( char * )
 */
void NachOS_Create() {		// System call 4
}


/*
 *  System call interface: OpenFileId Open( char * )
 */
void NachOS_Open() {		// System call 5
   int addr = machine->ReadRegister(4); // Dirección del string en memoria de usuario
   char filename[128];

   int val;
   for (int i = 0; i < 127; i++)
   {
      if (!machine->ReadMem(addr + i, 1, &val))
         break;
      filename[i] = (char)val;
      if (filename[i] == '\0')
         break;
   }
   filename[127] = '\0';

   int unixHandle = open(filename, O_RDONLY); // usar <fcntl.h> arriba
   if (unixHandle == -1)
   {
      machine->WriteRegister(2, -1);
      returnFromSystemCall();
      return;
   }

   int nachosHandle = currentThread->tabla->Open(unixHandle);
   machine->WriteRegister(2, nachosHandle);

   returnFromSystemCall();
}


/*
 *  System call interface: OpenFileId Write( char *, int, OpenFileId )
 */
void NachOS_Write() {		// System call 6
   int addr = machine->ReadRegister(4); // Dirección del buffer
   int size = machine->ReadRegister(5); // Tamaño del buffer
   int fd = machine->ReadRegister(6); // File descriptor

   // Validación: dirección válida y dentro del espacio de usuario
   int limit = currentThread->space->GetNumPages() * PageSize;

   if (addr < 0 || addr >= limit || (addr + size) > limit)
   {
      printf("ERROR: Write recibió dirección fuera de rango: %d\n", addr);
      machine->WriteRegister(2, -1);
      returnFromSystemCall();
      return;
   }
   
   char buffer[512];
   int val;

   if (size > 511)
      size = 511;

   // Leer el buffer de la memoria del usuario
   for (int i = 0; i < size; i++)
   {
      if (!machine->ReadMem(addr + i, 1, &val))
      {
         buffer[i] = '?'; // Error de lectura
      }
      else
      {
         buffer[i] = (char)val;
      }
   }

   // Escribir según el descriptor
   if (fd == ConsoleOutput || fd == ConsoleError)
   {
      for (int i = 0; i < size; i++)
      {
         printf("%c", buffer[i]);
      }
      machine->WriteRegister(2, size); // devuelve cantidad escrita
   }
   else
   {
      // Verificar si el descriptor es válido
      if (!currentThread->tabla->isOpened(fd))
      {
         machine->WriteRegister(2, -1); // error
      }
      else
      {
         int unixFD = currentThread->tabla->getUnixHandle(fd);
         int written = write(unixFD, buffer, size);
         machine->WriteRegister(2, written); // devuelve cantidad escrita
      }
   }
   // Avanzar el PC para evitar repetir el syscall
   returnFromSystemCall();
}

/*
 *  System call interface: OpenFileId Read( char *, int, OpenFileId )
 */
void NachOS_Read() {		// System call 7
   //printf("Entrando a NachOS_Read\n"); //Depuración
   int addr = machine->ReadRegister(4); // Dirección del buffer en user memory
   int size = machine->ReadRegister(5); // Cantidad de bytes a leer
   int fd = machine->ReadRegister(6);   // File descriptor

   char buffer[512];
   if (size > 511)
      size = 511;

   int bytesRead = 0;

   if (fd == ConsoleInput)
   {
      for (int i = 0; i < size; i++)
      {
         char c = getchar(); // leer de stdin
         buffer[i] = c;
         bytesRead++;
         if (c == '\n')
            break;
      }
   }
   else
   {
      if (!currentThread->tabla->isOpened(fd))
      {
         machine->WriteRegister(2, -1);
         returnFromSystemCall();
         return;
      }

      int unixFD = currentThread->tabla->getUnixHandle(fd);
      //printf("Ejecutando syscall Read() sobre fd=%d, esperando datos...\n", fd); //Depuración

      bytesRead = read(unixFD, buffer, size);
      if (bytesRead < 0)
      {
         printf("Error en read()\n");
         machine->WriteRegister(2, -1);
         returnFromSystemCall();
         return;
      }
   }

   // Escribir lo leído a memoria del usuario
   for (int i = 0; i < bytesRead; i++)
   {
      machine->WriteMem(addr + i, 1, (int)buffer[i]);
   }

   machine->WriteRegister(2, bytesRead); // devolver cantidad leída
   returnFromSystemCall();
}


/*
 *  System call interface: void Close( OpenFileId )
 */
void NachOS_Close() {		// System call 8
}


/*
 *  System call interface: void Fork( void (*func)() )
 */
void NachOS_Fork() {		// System call 9
   DEBUG('u', "Entering Fork System call\n");

   // Contador estático para asignar una pila distinta a cada hilo
   static int threadCounter = 0;
   int threadId = threadCounter++;

   // Crear el nuevo hilo
   Thread *newT = new Thread("Forked thread");

   // Crear un nuevo espacio de direcciones con la misma memoria compartida
   newT->space = new AddrSpace(currentThread->space, threadId);

   // Calcular la base de la pila para este hilo
   int stackBase = NumPhysPages * PageSize - (threadId + 1) * UserStackSize - 16;

   ForkArgs *args = new ForkArgs{(void *)(long)machine->ReadRegister(4), stackBase};

   // Lanzar el hilo con sus argumentos
   newT->Fork(Nachos_ForkThread, (void *)args);

   returnFromSystemCall();

   DEBUG('u', "Exiting Fork System call\n");
}


/*
 *  System call interface: void Yield()
 */
void NachOS_Yield() {		// System call 10
}


/*
 *  System call interface: Sem_t SemCreate( int )
 */
void NachOS_SemCreate() {		// System call 11
}


/*
 *  System call interface: int SemDestroy( Sem_t )
 */
void NachOS_SemDestroy() {		// System call 12
}


/*
 *  System call interface: int SemSignal( Sem_t )
 */
void NachOS_SemSignal() {		// System call 13
}


/*
 *  System call interface: int SemWait( Sem_t )
 */
void NachOS_SemWait() {		// System call 14
}


/*
 *  System call interface: Lock_t LockCreate( int )
 */
void NachOS_LockCreate() {		// System call 15
}


/*
 *  System call interface: int LockDestroy( Lock_t )
 */
void NachOS_LockDestroy() {		// System call 16
}


/*
 *  System call interface: int LockAcquire( Lock_t )
 */
void NachOS_LockAcquire() {		// System call 17
}


/*
 *  System call interface: int LockRelease( Lock_t )
 */
void NachOS_LockRelease() {		// System call 18
}


/*
 *  System call interface: Cond_t LockCreate( int )
 */
void NachOS_CondCreate() {		// System call 19
}


/*
 *  System call interface: int CondDestroy( Cond_t )
 */
void NachOS_CondDestroy() {		// System call 20
}


/*
 *  System call interface: int CondSignal( Cond_t )
 */
void NachOS_CondSignal() {		// System call 21
}


/*
 *  System call interface: int CondWait( Cond_t )
 */
void NachOS_CondWait() {		// System call 22
}


/*
 *  System call interface: int CondBroadcast( Cond_t )
 */
void NachOS_CondBroadcast() {		// System call 23
}


/*
 *  System call interface: Socket_t Socket( int, int )
 */
void NachOS_Socket() {			// System call 30
   int family = machine->ReadRegister(4);   // AF_INET_NachOS o AF_INET6_NachOS
   int type = machine->ReadRegister(5);     // SOCK_STREAM_NachOS o SOCK_DGRAM_NachOS
   int protocol = machine->ReadRegister(6); // usualmente 0

   int af = (family == AF_INET_NachOS) ? AF_INET : AF_INET6;
   int socktype = (type == SOCK_STREAM_NachOS) ? SOCK_STREAM : SOCK_DGRAM;

   int sockfd = socket(af, socktype, protocol);
   if (sockfd < 0)
   {
      machine->WriteRegister(2, -1);
   }
   else
   {
      int nachosFD = currentThread->tabla->Open(sockfd);
      machine->WriteRegister(2, nachosFD);
   }

   returnFromSystemCall();
}

/*
 *  System call interface: Socket_t Connect( char *, int )
 */
void NachOS_Connect() {		// System call 31
   int sockfd = machine->ReadRegister(4);
   int addrUser = machine->ReadRegister(5); // Dirección de la IP en memoria de usuario
   int port = machine->ReadRegister(6);

   char ip[64];
   int val;
   for (int i = 0; i < 63; ++i)
   {
      if (!machine->ReadMem(addrUser + i, 1, &val))
         break;
      ip[i] = (char)val;
      if (ip[i] == '\0')
         break;
   }
   ip[63] = '\0';
   /*Pruebas locales
   const char *ip = "127.0.0.1";
   port = 8080;*/
   if (!currentThread->tabla->isOpened(sockfd))
   {
      machine->WriteRegister(2, -1);
      returnFromSystemCall();
      return;
   }

   int unixfd = currentThread->tabla->getUnixHandle(sockfd);
   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
   {
      machine->WriteRegister(2, -1);
      returnFromSystemCall();
      return;
   }

   int res = connect(unixfd, (struct sockaddr *)&addr, sizeof(addr));
   machine->WriteRegister(2, (res < 0) ? -1 : 0);

   returnFromSystemCall();
}


/*
 *  System call interface: int Bind( Socket_t, int )
 */
void NachOS_Bind() {		// System call 32
}


/*
 *  System call interface: int Listen( Socket_t, int )
 */
void NachOS_Listen() {		// System call 33
}


/*
 *  System call interface: int Accept( Socket_t )
 */
void NachOS_Accept() {		// System call 34
}


/*
 *  System call interface: int Shutdown( Socket_t, int )
 */
void NachOS_Shutdown() {	// System call 25
}

/**
 * Return from a system call
 */
void returnFromSystemCall()
{
   // PrevPC <- PC actual
   machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));

   // PC <- NextPC actual (avanzamos al siguiente paso del programa)
   machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));

   // NextPC <- NextPC + 4 (preparar la siguiente instrucción)
   machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch ( which ) {

       case SyscallException:
          switch ( type ) {
             case SC_Halt:		// System call # 0
                NachOS_Halt();
                break;
             case SC_Exit:		// System call # 1
                NachOS_Exit();
                break;
             case SC_Exec:		// System call # 2
                NachOS_Exec();
                break;
             case SC_Join:		// System call # 3
                NachOS_Join();
                break;

             case SC_Create:		// System call # 4
                NachOS_Create();
                break;
             case SC_Open:		// System call # 5
                NachOS_Open();
                break;
             case SC_Read:		// System call # 6
                NachOS_Read();
                break;
             case SC_Write:		// System call # 7
                NachOS_Write();
                break;
             case SC_Close:		// System call # 8
                NachOS_Close();
                break;

             case SC_Fork:		// System call # 9
                NachOS_Fork();
                break;
             case SC_Yield:		// System call # 10
                NachOS_Yield();
                break;

             case SC_SemCreate:         // System call # 11
                NachOS_SemCreate();
                break;
             case SC_SemDestroy:        // System call # 12
                NachOS_SemDestroy();
                break;
             case SC_SemSignal:         // System call # 13
                NachOS_SemSignal();
                break;
             case SC_SemWait:           // System call # 14
                NachOS_SemWait();
                break;

             case SC_LckCreate:         // System call # 15
                NachOS_LockCreate();
                break;
             case SC_LckDestroy:        // System call # 16
                NachOS_LockDestroy();
                break;
             case SC_LckAcquire:         // System call # 17
                NachOS_LockAcquire();
                break;
             case SC_LckRelease:           // System call # 18
                NachOS_LockRelease();
                break;

             case SC_CondCreate:         // System call # 19
                NachOS_CondCreate();
                break;
             case SC_CondDestroy:        // System call # 20
                NachOS_CondDestroy();
                break;
             case SC_CondSignal:         // System call # 21
                NachOS_CondSignal();
                break;
             case SC_CondWait:           // System call # 22
                NachOS_CondWait();
                break;
             case SC_CondBroadcast:           // System call # 23
                NachOS_CondBroadcast();
                break;

             case SC_Socket:	// System call # 30
		NachOS_Socket();
               break;
             case SC_Connect:	// System call # 31
		NachOS_Connect();
               break;
             case SC_Bind:	// System call # 32
		NachOS_Bind();
               break;
             case SC_Listen:	// System call # 33
		NachOS_Listen();
               break;
             case SC_Accept:	// System call # 32
		NachOS_Accept();
               break;
             case SC_Shutdown:	// System call # 33
		NachOS_Shutdown();
               break;

             default:
                printf("Unexpected syscall exception %d\n", type );
                ASSERT( false );
                break;
          }
          break;

       case PageFaultException: {
          break;
       }

       case ReadOnlyException:
          printf( "Read Only exception (%d)\n", which );
          ASSERT( false );
          break;

       case BusErrorException:
          printf( "Bus error exception (%d)\n", which );
          ASSERT( false );
          break;

       case AddressErrorException:
          printf( "Address error exception (%d)\n", which );
          ASSERT( false );
          break;

       case OverflowException:
          printf( "Overflow exception (%d)\n", which );
          ASSERT( false );
          break;

       case IllegalInstrException:
          printf( "Ilegal instruction exception (%d)\n", which );
          ASSERT( false );
          break;

       default:
          printf( "Unexpected exception %d\n", which );
          ASSERT( false );
          break;
    }

}
