
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <linux/shm.h>
#include <sys/stat.h>

#include <sanitizer/coverage_interface.h>

#include "sanitize_converage.h"
#include "signal_number.h"

/// -fsanitize-coverage-whitelist=



static int __sancov_trace_pc_switch = 0;
static const size_t __sancov_trace_pc_map_max = 1 << 16;
static const size_t __sancov_trace_pc_map_size = __sancov_trace_pc_map_max * sizeof(__sancov_trace_pc_map);
static __sancov_trace_pc_map __sancov_trace_pc_table[__sancov_trace_pc_map_max] = {0};
static unsigned long __sancov_trace_pc_index = 0;
static unsigned long __sancov_current_all_guard_count = 0;
static pid_t __sancov_father_pid = 0;
static unsigned long  __sancov_fuzz_loop = 0;


ATTRIBUTE_NO_SANITIZE_ALL
void __sanitizer_cov_trace_pc_guard_init(uint32_t *start,uint32_t *stop) {
    uint_t id = 0;

    if (start == stop || *start)
        return;

    __sancov_father_pid = getppid();

    if (__sancov_father_pid > 1) {
        union sigval send_sigval;
        send_sigval.sival_int = getpid();

        sigqueue(__sancov_father_pid,SIGNAL_CREATE_FUZZER_TARGET,send_sigval);
    } else {
        //memset(__sancov_trace_pc_table,0,__sancov_trace_pc_map_size);
    }

    __sancov_current_all_guard_count = (stop - start);

    printf("Sanitizer All Coverage edges: 0x%X \n",__sancov_current_all_guard_count);

    for (uint32_t* edge_id = start; edge_id < stop; ++edge_id)
        *edge_id = ++id;
}

ATTRIBUTE_NO_SANITIZE_ALL
void __sanitizer_cov_trace_pc_guard(uint32_t *guard,uint32_t count,uint32_t function_id) {
    uint_t edge_address = (uint_t)guard;
    uint_t edge_id = *guard;

    if (!edge_id || !__sancov_trace_pc_switch)
        return;

    void *PC = __builtin_return_address(0);
    char symbolize_data[1024];
    char current_function_name[512];

    #ifdef IS_DEBUF_MODE
    __sanitizer_symbolize_pc(PC, "%p %F", symbolize_data, sizeof(symbolize_data));
    __sanitizer_symbolize_pc(PC, "%F", current_function_name, sizeof(current_function_name));

    printf("%d Sanitizer Trace PC Guard(count=%d) : %x PC %s\n",
            __sancov_trace_pc_index,count,edge_id, symbolize_data);
    #endif

    __sancov_trace_pc_table[__sancov_trace_pc_index].current_edge_id = edge_id;
    __sancov_trace_pc_table[__sancov_trace_pc_index].current_function_edge_count = count;
    __sancov_trace_pc_table[__sancov_trace_pc_index].current_function_entry = function_id;

    __sancov_trace_pc_index++;
}

ATTRIBUTE_NO_SANITIZE_ALL
void __sanitizer_enter(void) {
    memset(&__sancov_trace_pc_table,0,sizeof(__sancov_trace_pc_table));

    __sancov_trace_pc_index = 0;
    __sancov_trace_pc_switch = 1;
}

ATTRIBUTE_NO_SANITIZE_ALL
void __sanitizer_exit(void) {
    uint_t trace_pc_count = __sancov_trace_pc_index;
    uint_t pipe_write_size = trace_pc_count * sizeof(__sancov_trace_pc_map);

    #ifdef IS_DEBUF_MODE
    printf("Sanitizer All Execute Edges : %d \n",__sancov_trace_pc_index);
    #endif

    char save_dir[MAX_PATH_SIZE] = {0};

    sprintf(save_dir,"./temp_%d_%d",__sancov_father_pid,getpid());

    if (!opendir(save_dir))
        mkdir(save_dir,0777);

    char save_coverage_path[MAX_PATH_SIZE] = {0};

    sprintf(save_coverage_path,"%s/%d.dat",save_dir,__sancov_fuzz_loop);

    int save_data_handle = open(save_coverage_path,O_RDWR | O_CREAT,S_IRUSR | S_IWUSR | S_IROTH);

    write(save_data_handle,&trace_pc_count,sizeof(trace_pc_count));
    write(save_data_handle,&__sancov_trace_pc_table,pipe_write_size);
    close(save_data_handle);

    if (__sancov_father_pid > 1) {
        union sigval send_sigval;
        send_sigval.sival_int = __sancov_fuzz_loop;

        sigqueue(__sancov_father_pid,SIGNAL_FUZZ_ONCE,send_sigval);

        __sancov_fuzz_loop++;
    } else {
    }

    //memset(__sancov_trace_pc_table,0,pipe_write_size);

    __sancov_trace_pc_index = 0;
    __sancov_trace_pc_switch = 0;
}





