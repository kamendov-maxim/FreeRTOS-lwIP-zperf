#ifndef _DEFINES
#define _DEFINES

// #include "shell/fsl_shell.h"
typedef enum  { 
  kStatus_SHELL_Success = 0, 
  kStatus_SHELL_Error = 1,
  kStatus_SHELL_OpenWriteHandleFailed = 2,
  kStatus_SHELL_OpenReadHandleFailed = 3,
          kStatus_SHELL_PrintCmdHelp = 4,
    kStatus_SHELL_PrintAllHelp = 5

} shell_status_t;

// #define _shell_status enum shell_status_t

// typedef enum _shell_status
// {
    // kStatus_SHELL_Success               = kStatus_Success,                    /*!< Success */
    // kStatus_SHELL_Error                 = MAKE_STATUS(kStatusGroup_SHELL, 1), /*!< Failed */
    // kStatus_SHELL_OpenWriteHandleFailed = MAKE_STATUS(kStatusGroup_SHELL, 2), /*!< Open write handle failed */
    // kStatus_SHELL_OpenReadHandleFailed  = MAKE_STATUS(kStatusGroup_SHELL, 3), /*!< Open read handle failed */
    // kStatus_SHELL_PrintCmdHelp = MAKE_STATUS(kStatusGroup_SHELL, 4), /*!< Print current command help information */
    // kStatus_SHELL_PrintAllHelp = MAKE_STATUS(kStatusGroup_SHELL, 5), /*!< Print all command help information */
    // kStatus_SHELL_PrintCmdHelp = 4,
    // kStatus_SHELL_PrintAllHelp = 5
    
// } shell_status_t;



typedef void *serial_handle_t;
#define SHELL_BUFFER_SIZE (64U)
#define SERIAL_MANAGER_WRITE_HANDLE_SIZE (4U)
#define SERIAL_MANAGER_READ_HANDLE_SIZE (4U)
#define SHELL_HISTORY_COUNT (3U)

#define OSA_TASK_HANDLE_SIZE (12U)
#define OSA_SEM_HANDLE_SIZE (4U)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
#define SHELL_HANDLE_SIZE                                                                                              \
    (112U + 8U + SHELL_HISTORY_COUNT * SHELL_BUFFER_SIZE + SHELL_BUFFER_SIZE + SERIAL_MANAGER_READ_HANDLE_SIZE +       \
     SERIAL_MANAGER_WRITE_HANDLE_SIZE)
#else
#define SHELL_HANDLE_SIZE                                                                                              \
    (112U + OSA_SEM_HANDLE_SIZE + OSA_TASK_HANDLE_SIZE + SHELL_HISTORY_COUNT * SHELL_BUFFER_SIZE + SHELL_BUFFER_SIZE + \
     SERIAL_MANAGER_READ_HANDLE_SIZE + SERIAL_MANAGER_WRITE_HANDLE_SIZE)
#endif
#define SHELL_HANDLE_DEFINE(name) uint32_t name[((SHELL_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))]

// shell_status_t SHELL_Init(shell_handle_t shellHandle, serial_handle_t serialHandle, char *prompt)
// {
//     shell_context_handle_t *shellContextHandle;
//     serial_manager_status_t status = kStatus_SerialManager_Error;
//     (void)status;
//
//     assert(shellHandle);
// #if !(!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
//     assert(serialHandle);
// #endif
//     assert(prompt);
//
//     assert(SHELL_HANDLE_SIZE >= sizeof(shell_context_handle_t));
//
//     shellContextHandle = (shell_context_handle_t *)shellHandle;
//
//     /* memory set for shellHandle */
//     (void)memset(shellHandle, 0, SHELL_HANDLE_SIZE);
//
// #if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
//     if (NULL == serialHandle)
//     {
//     }
//     else
// #endif
//     {
// #if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
//
// #if defined(OSA_USED)
//
// #if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
//         COMMON_TASK_init();
// #else
//         if (KOSA_StatusSuccess != OSA_SemaphoreCreate((osa_semaphore_handle_t)shellContextHandle->semphore, 0))
//         {
//             return kStatus_SHELL_Error;
//         }
//
//         if (KOSA_StatusSuccess !=
//             OSA_TaskCreate((osa_task_handle_t)shellContextHandle->taskId, OSA_TASK(SHELL_Task), shellContextHandle))
//         {
//             return kStatus_SHELL_Error;
//         }
// #endif
//
// #endif
//
// #endif
//     }
//
//     shellContextHandle->prompt = prompt;
//     shellContextHandle->serialHandle = serialHandle;
//
// #if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
//     if (NULL == serialHandle)
//     {
//     }
//     else
// #endif
//     {
//         shellContextHandle->serialWriteHandle = (serial_write_handle_t)&shellContextHandle->serialWriteHandleBuffer[0];
//         status = SerialManager_OpenWriteHandle(shellContextHandle->serialHandle, shellContextHandle->serialWriteHandle);
//         assert(kStatus_SerialManager_Success == status);
//
//         shellContextHandle->serialReadHandle = (serial_read_handle_t)&shellContextHandle->serialReadHandleBuffer[0];
//         status = SerialManager_OpenReadHandle(shellContextHandle->serialHandle, shellContextHandle->serialReadHandle);
//         assert(kStatus_SerialManager_Success == status);
//
// #if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
//         status = SerialManager_InstallRxCallback(shellContextHandle->serialReadHandle, SHELL_SerialManagerRxCallback,
//                                                  shellContextHandle);
//         assert(kStatus_SerialManager_Success == status);
// #endif
//         (void)status;
//     }
//
//     // (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(help));
//     // (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(exit));
// #if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
// #if (SHELL_ECHO > 0)
//     (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(echo));
// #endif
// #endif
//
//     (void)SHELL_Write(shellContextHandle, (char *)"\r\nSHELL build: ", strlen("\r\nSHELL build: "));
//     (void)SHELL_Write(shellContextHandle, (char *)__DATE__, strlen(__DATE__));
//     (void)SHELL_Write(shellContextHandle, (char *)"\r\nCopyright  2020  NXP\r\n",
//                       strlen("\r\nCopyright  2020  NXP\r\n"));
//     (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt, strlen(shellContextHandle->prompt));
//
//     return kStatus_SHELL_Success;
// }

#endif // !_DEFINES
