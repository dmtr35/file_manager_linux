#define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <utime.h>


#include "../header.h"


void change_permission(char *new_permission, char *path, char *name_file_row)
{
    size_t size_full_path = strlen(path) + strlen(name_file_row) + 2;
    char *full_path = malloc(size_full_path * sizeof(char));
    snprintf(full_path, size_full_path, "%s/%s", path, name_file_row);

    
    mode_t new_mode = parmission(new_permission, full_path);

    struct stat file_info;
    if (lstat(full_path, &file_info) == 0) {
        if (S_ISDIR(file_info.st_mode)) {
            chmod(full_path, new_mode);
        } else if (S_ISREG(file_info.st_mode)) {
            chmod(full_path, new_mode);
        } else if (S_ISLNK(file_info.st_mode)) {
            chmod(full_path, new_mode);
            free(full_path);
            return;
        } 
    }


    free(full_path);
}



mode_t parmission(char *new_permission, char *full_path)
{
    mode_t existing_mode;
    mode_t new_mode;

    mode_t read_other = S_IROTH;
    mode_t write_other = S_IWOTH;
    mode_t exec_other = S_IXOTH;

    mode_t read_group = S_IRGRP;
    mode_t write_group = S_IWGRP;
    mode_t exec_group = S_IXGRP;

    mode_t read_user = S_IRUSR;
    mode_t write_user = S_IWUSR;
    mode_t exec_user = S_IXUSR;

    mode_t setuid_bit = S_ISUID;                                // бит setuid
    mode_t setgid_bit = S_ISGID;                                // бит setgid
    mode_t sticky_bit = S_ISVTX;                                // бит sticky





    struct stat file_info;
    if(lstat(full_path, &file_info) == 0) {
        mode_t existing_mode = file_info.st_mode;
    }

    if (strcmp(new_permission, "000") == 0 || strcmp(new_permission, "0000") == 0) {
        new_mode = 0;
    } else if (strcmp(new_permission, "001") == 0 || strcmp(new_permission, "0001") == 0) {
        new_mode = exec_other;
    } else if (strcmp(new_permission, "002") == 0 || strcmp(new_permission, "0002") == 0) {
        new_mode = write_other;
    } else if (strcmp(new_permission, "003") == 0 || strcmp(new_permission, "0003") == 0) {
        new_mode = write_other | exec_other;
    } else if (strcmp(new_permission, "004") == 0 || strcmp(new_permission, "0004") == 0) {
        new_mode = read_other;
    } else if (strcmp(new_permission, "005") == 0 || strcmp(new_permission, "0005") == 0) {
        new_mode = read_other | exec_other;
    } else if (strcmp(new_permission, "006") == 0 || strcmp(new_permission, "0006") == 0) {
        new_mode = read_other | write_other;
    } else if (strcmp(new_permission, "007") == 0 || strcmp(new_permission, "0007") == 0) {              
        new_mode = read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "010") == 0 || strcmp(new_permission, "0010") == 0) {
        new_mode = exec_group;
    } else if (strcmp(new_permission, "011") == 0 || strcmp(new_permission, "0011") == 0) {
        new_mode = exec_group | exec_other;
    } else if (strcmp(new_permission, "012") == 0 || strcmp(new_permission, "0012") == 0) {
        new_mode = exec_group | write_other;
    } else if (strcmp(new_permission, "013") == 0 || strcmp(new_permission, "0013") == 0) {
        new_mode = exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "014") == 0 || strcmp(new_permission, "0014") == 0) {
        new_mode = exec_group | read_other;
    } else if (strcmp(new_permission, "015") == 0 || strcmp(new_permission, "0015") == 0) {
        new_mode = exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "016") == 0 || strcmp(new_permission, "0016") == 0) {
        new_mode = exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "017") == 0 || strcmp(new_permission, "0017") == 0) {               
        new_mode = exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "020") == 0 || strcmp(new_permission, "0020") == 0) {
        new_mode = write_group;
    } else if (strcmp(new_permission, "021") == 0 || strcmp(new_permission, "0021") == 0) {
        new_mode = write_group | exec_other;
    } else if (strcmp(new_permission, "022") == 0 || strcmp(new_permission, "0022") == 0) {
        new_mode = write_group | write_other;
    } else if (strcmp(new_permission, "023") == 0 || strcmp(new_permission, "0023") == 0) {
        new_mode = write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "024") == 0 || strcmp(new_permission, "0024") == 0) {
        new_mode = write_group | read_other;
    } else if (strcmp(new_permission, "025") == 0 || strcmp(new_permission, "0025") == 0) {
        new_mode = write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "026") == 0 || strcmp(new_permission, "0026") == 0) {
        new_mode = write_group | read_other | write_other;
    } else if (strcmp(new_permission, "027") == 0 || strcmp(new_permission, "0027") == 0) {                
        new_mode = write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "030") == 0 || strcmp(new_permission, "0030") == 0) {
        new_mode = write_group | exec_group;
    } else if (strcmp(new_permission, "031") == 0 || strcmp(new_permission, "0031") == 0) {
        new_mode = write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "032") == 0 || strcmp(new_permission, "0032") == 0) {
        new_mode = write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "033") == 0 || strcmp(new_permission, "0033") == 0) {
        new_mode = write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "034") == 0 || strcmp(new_permission, "0034") == 0) {
        new_mode = write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "035") == 0 || strcmp(new_permission, "0035") == 0) {
        new_mode = write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "036") == 0 || strcmp(new_permission, "0036") == 0) {
        new_mode = write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "037") == 0 || strcmp(new_permission, "0037") == 0) {                
        new_mode = write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "040") == 0 || strcmp(new_permission, "0040") == 0) {
        new_mode = read_group;
    } else if (strcmp(new_permission, "041") == 0 || strcmp(new_permission, "0041") == 0) {
        new_mode = read_group | exec_other;
    } else if (strcmp(new_permission, "042") == 0 || strcmp(new_permission, "0042") == 0) {
        new_mode = read_group | write_other;
    } else if (strcmp(new_permission, "043") == 0 || strcmp(new_permission, "0043") == 0) {
        new_mode = read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "044") == 0 || strcmp(new_permission, "0044") == 0) {
        new_mode = read_group | read_other;
    } else if (strcmp(new_permission, "045") == 0 || strcmp(new_permission, "0045") == 0) {
        new_mode = read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "046") == 0 || strcmp(new_permission, "0046") == 0) {
        new_mode = read_group | read_other | write_other;
    } else if (strcmp(new_permission, "047") == 0 || strcmp(new_permission, "0047") == 0) {                
        new_mode = read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "050") == 0 || strcmp(new_permission, "0050") == 0) {
        new_mode = read_group | exec_group;
    } else if (strcmp(new_permission, "051") == 0 || strcmp(new_permission, "0051") == 0) {
        new_mode = read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "052") == 0 || strcmp(new_permission, "0052") == 0) {
        new_mode = read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "053") == 0 || strcmp(new_permission, "0053") == 0) {
        new_mode = read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "054") == 0 || strcmp(new_permission, "0054") == 0) {
        new_mode = read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "055") == 0 || strcmp(new_permission, "0055") == 0) {
        new_mode = read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "056") == 0 || strcmp(new_permission, "0056") == 0) {
        new_mode = read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "057") == 0 || strcmp(new_permission, "0057") == 0) {                
        new_mode = read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "060") == 0 || strcmp(new_permission, "0060") == 0) {
        new_mode = read_group | write_group;
    } else if (strcmp(new_permission, "061") == 0 || strcmp(new_permission, "0061") == 0) {
        new_mode = read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "062") == 0 || strcmp(new_permission, "0062") == 0) {
        new_mode = read_group | write_group | write_other;
    } else if (strcmp(new_permission, "063") == 0 || strcmp(new_permission, "0063") == 0) {
        new_mode = read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "064") == 0 || strcmp(new_permission, "0064") == 0) {
        new_mode = read_group | write_group | read_other;
    } else if (strcmp(new_permission, "065") == 0 || strcmp(new_permission, "0065") == 0) {
        new_mode = read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "066") == 0 || strcmp(new_permission, "0066") == 0) {
        new_mode = read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "067") == 0 || strcmp(new_permission, "0067") == 0) {                 
        new_mode = read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "070") == 0 || strcmp(new_permission, "0070") == 0) {
        new_mode = read_group | write_group | exec_group;
    } else if (strcmp(new_permission, "071") == 0 || strcmp(new_permission, "0071") == 0) {
        new_mode = read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "072") == 0 || strcmp(new_permission, "0072") == 0) {
        new_mode = read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "073") == 0 || strcmp(new_permission, "0073") == 0) {
        new_mode = read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "074") == 0 || strcmp(new_permission, "0074") == 0) {
        new_mode = read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "075") == 0 || strcmp(new_permission, "0075") == 0) {
        new_mode = read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "076") == 0 || strcmp(new_permission, "0076") == 0) {
        new_mode = read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "077") == 0 || strcmp(new_permission, "0077") == 0) {                 
        new_mode = read_group | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    // ====================================================================================

    else if (strcmp(new_permission, "100") == 0 || strcmp(new_permission, "0100") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "101") == 0 || strcmp(new_permission, "0101") == 0) {
        new_mode = exec_user | exec_other;
    } else if (strcmp(new_permission, "102") == 0 || strcmp(new_permission, "0102") == 0) {
        new_mode = exec_user | write_other;
    } else if (strcmp(new_permission, "103") == 0 || strcmp(new_permission, "0103") == 0) {
        new_mode = exec_user | write_other | exec_other;
    } else if (strcmp(new_permission, "104") == 0 || strcmp(new_permission, "0104") == 0) {
        new_mode = exec_user | read_other;
    } else if (strcmp(new_permission, "105") == 0 || strcmp(new_permission, "0105") == 0) {
        new_mode = exec_user | read_other | exec_other;
    } else if (strcmp(new_permission, "106") == 0 || strcmp(new_permission, "0106") == 0) {
        new_mode = exec_user | read_other | write_other;
    } else if (strcmp(new_permission, "107") == 0 || strcmp(new_permission, "0107") == 0) {
        new_mode = exec_user | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "110") == 0 || strcmp(new_permission, "0110") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "111") == 0 || strcmp(new_permission, "0111") == 0) {
        new_mode = exec_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "112") == 0 || strcmp(new_permission, "0112") == 0) {
        new_mode = exec_user | exec_group | write_other;
    } else if (strcmp(new_permission, "113") == 0 || strcmp(new_permission, "0113") == 0) {
        new_mode = exec_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "114") == 0 || strcmp(new_permission, "0114") == 0) {
        new_mode = exec_user | exec_group | read_other;
    } else if (strcmp(new_permission, "115") == 0 || strcmp(new_permission, "0115") == 0) {
        new_mode = exec_user | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "116") == 0 || strcmp(new_permission, "0116") == 0) {
        new_mode = exec_user | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "117") == 0 || strcmp(new_permission, "0117") == 0) {
        new_mode = exec_user | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "120") == 0 || strcmp(new_permission, "0120") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "121") == 0 || strcmp(new_permission, "0121") == 0) {
        new_mode = exec_user | write_group | exec_other;
    } else if (strcmp(new_permission, "122") == 0 || strcmp(new_permission, "0122") == 0) {
        new_mode = exec_user | write_group | write_other;
    } else if (strcmp(new_permission, "123") == 0 || strcmp(new_permission, "0123") == 0) {
        new_mode = exec_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "124") == 0 || strcmp(new_permission, "0124") == 0) {
        new_mode = exec_user | write_group | read_other;
    } else if (strcmp(new_permission, "125") == 0 || strcmp(new_permission, "0125") == 0) {
        new_mode = exec_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "126") == 0 || strcmp(new_permission, "0126") == 0) {
        new_mode = exec_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "127") == 0 || strcmp(new_permission, "0127") == 0) {
        new_mode = exec_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "130") == 0 || strcmp(new_permission, "0130") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "131") == 0 || strcmp(new_permission, "0131") == 0) {
        new_mode = exec_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "132") == 0 || strcmp(new_permission, "0132") == 0) {
        new_mode = exec_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "133") == 0 || strcmp(new_permission, "0133") == 0) {
        new_mode = exec_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "134") == 0 || strcmp(new_permission, "0134") == 0) {
        new_mode = exec_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "135") == 0 || strcmp(new_permission, "0135") == 0) {
        new_mode = exec_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "136") == 0 || strcmp(new_permission, "0136") == 0) {
        new_mode = exec_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "137") == 0 || strcmp(new_permission, "0137") == 0) {
        new_mode = exec_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "140") == 0 || strcmp(new_permission, "0140") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "141") == 0 || strcmp(new_permission, "0141") == 0) {
        new_mode = exec_user | read_group | exec_other;
    } else if (strcmp(new_permission, "142") == 0 || strcmp(new_permission, "0142") == 0) {
        new_mode = exec_user | read_group | write_other;
    } else if (strcmp(new_permission, "143") == 0 || strcmp(new_permission, "0143") == 0) {
        new_mode = exec_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "144") == 0 || strcmp(new_permission, "0144") == 0) {
        new_mode = exec_user | read_group | read_other;
    } else if (strcmp(new_permission, "145") == 0 || strcmp(new_permission, "0145") == 0) {
        new_mode = exec_user | read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "146") == 0 || strcmp(new_permission, "0146") == 0) {
        new_mode = exec_user | read_group | read_other | write_other;
    } else if (strcmp(new_permission, "147") == 0 || strcmp(new_permission, "0147") == 0) {
        new_mode = exec_user | read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "150") == 0 || strcmp(new_permission, "0150") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "151") == 0 || strcmp(new_permission, "0151") == 0) {
        new_mode = exec_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "152") == 0 || strcmp(new_permission, "0152") == 0) {
        new_mode = exec_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "153") == 0 || strcmp(new_permission, "0153") == 0) {
        new_mode = exec_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "154") == 0 || strcmp(new_permission, "0154") == 0) {
        new_mode = exec_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "155") == 0 || strcmp(new_permission, "0155") == 0) {
        new_mode = exec_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "156") == 0 || strcmp(new_permission, "0156") == 0) {
        new_mode = exec_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "157") == 0 || strcmp(new_permission, "0157") == 0) {
        new_mode = exec_user | read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "160") == 0 || strcmp(new_permission, "0160") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "161") == 0 || strcmp(new_permission, "0161") == 0) {
        new_mode = exec_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "162") == 0 || strcmp(new_permission, "0162") == 0) {
        new_mode = exec_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "163") == 0 || strcmp(new_permission, "0163") == 0) {
        new_mode = exec_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "164") == 0 || strcmp(new_permission, "0164") == 0) {
        new_mode = exec_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "165") == 0 || strcmp(new_permission, "0165") == 0) {
        new_mode = exec_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "166") == 0 || strcmp(new_permission, "0166") == 0) {
        new_mode = exec_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "167") == 0 || strcmp(new_permission, "0167") == 0) {
        new_mode = exec_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "170") == 0 || strcmp(new_permission, "0170") == 0) {
        new_mode = exec_user;
    } else if (strcmp(new_permission, "171") == 0 || strcmp(new_permission, "0171") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "172") == 0 || strcmp(new_permission, "0172") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "173") == 0 || strcmp(new_permission, "0173") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "174") == 0 || strcmp(new_permission, "0174") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "175") == 0 || strcmp(new_permission, "0175") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "176") == 0 || strcmp(new_permission, "0176") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "177") == 0 || strcmp(new_permission, "0177") == 0) {
        new_mode = exec_user | read_group | write_group | exec_group | read_other |write_other | exec_other;   
    }

    // =====================================================================================


    else if (strcmp(new_permission, "200") == 0 || strcmp(new_permission, "0200") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "201") == 0 || strcmp(new_permission, "0201") == 0) {
        new_mode = write_user | exec_other;
    } else if (strcmp(new_permission, "202") == 0 || strcmp(new_permission, "0202") == 0) {
        new_mode = write_user | write_other;
    } else if (strcmp(new_permission, "203") == 0 || strcmp(new_permission, "0203") == 0) {
        new_mode = write_user | write_other | exec_other;
    } else if (strcmp(new_permission, "204") == 0 || strcmp(new_permission, "0204") == 0) {
        new_mode = write_user | read_other;
    } else if (strcmp(new_permission, "205") == 0 || strcmp(new_permission, "0205") == 0) {
        new_mode = write_user | read_other | exec_other;
    } else if (strcmp(new_permission, "206") == 0 || strcmp(new_permission, "0206") == 0) {
        new_mode = write_user | read_other | write_other;
    } else if (strcmp(new_permission, "207") == 0 || strcmp(new_permission, "0207") == 0) {
        new_mode = write_user | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "210") == 0 || strcmp(new_permission, "0210") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "211") == 0 || strcmp(new_permission, "0211") == 0) {
        new_mode = write_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "212") == 0 || strcmp(new_permission, "0212") == 0) {
        new_mode = write_user | exec_group | write_other;
    } else if (strcmp(new_permission, "213") == 0 || strcmp(new_permission, "0213") == 0) {
        new_mode = write_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "214") == 0 || strcmp(new_permission, "0214") == 0) {
        new_mode = write_user | exec_group | read_other;
    } else if (strcmp(new_permission, "215") == 0 || strcmp(new_permission, "0215") == 0) {
        new_mode = write_user | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "216") == 0 || strcmp(new_permission, "0216") == 0) {
        new_mode = write_user | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "217") == 0 || strcmp(new_permission, "0217") == 0) {
        new_mode = write_user | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "220") == 0 || strcmp(new_permission, "0220") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "221") == 0 || strcmp(new_permission, "0221") == 0) {
        new_mode = write_user | write_group | exec_other;
    } else if (strcmp(new_permission, "222") == 0 || strcmp(new_permission, "0222") == 0) {
        new_mode = write_user | write_group | write_other;
    } else if (strcmp(new_permission, "223") == 0 || strcmp(new_permission, "0223") == 0) {
        new_mode = write_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "224") == 0 || strcmp(new_permission, "0224") == 0) {
        new_mode = write_user | write_group | read_other;
    } else if (strcmp(new_permission, "225") == 0 || strcmp(new_permission, "0225") == 0) {
        new_mode = write_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "226") == 0 || strcmp(new_permission, "0226") == 0) {
        new_mode = write_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "227") == 0 || strcmp(new_permission, "0227") == 0) {
        new_mode = write_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "230") == 0 || strcmp(new_permission, "0230") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "231") == 0 || strcmp(new_permission, "0231") == 0) {
        new_mode = write_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "232") == 0 || strcmp(new_permission, "0232") == 0) {
        new_mode = write_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "233") == 0 || strcmp(new_permission, "0233") == 0) {
        new_mode = write_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "234") == 0 || strcmp(new_permission, "0234") == 0) {
        new_mode = write_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "235") == 0 || strcmp(new_permission, "0235") == 0) {
        new_mode = write_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "236") == 0 || strcmp(new_permission, "0236") == 0) {
        new_mode = write_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "237") == 0 || strcmp(new_permission, "0237") == 0) {
        new_mode = write_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "240") == 0 || strcmp(new_permission, "0240") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "241") == 0 || strcmp(new_permission, "0241") == 0) {
        new_mode = write_user | read_group | exec_other;
    } else if (strcmp(new_permission, "242") == 0 || strcmp(new_permission, "0242") == 0) {
        new_mode = write_user | read_group | write_other;
    } else if (strcmp(new_permission, "243") == 0 || strcmp(new_permission, "0243") == 0) {
        new_mode = write_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "244") == 0 || strcmp(new_permission, "0244") == 0) {
        new_mode = write_user | read_group | read_other;
    } else if (strcmp(new_permission, "245") == 0 || strcmp(new_permission, "0245") == 0) {
        new_mode = write_user | read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "246") == 0 || strcmp(new_permission, "0246") == 0) {
        new_mode = write_user | read_group | read_other | write_other;
    } else if (strcmp(new_permission, "247") == 0 || strcmp(new_permission, "0247") == 0) {
        new_mode = write_user | read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "250") == 0 || strcmp(new_permission, "0250") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "251") == 0 || strcmp(new_permission, "0251") == 0) {
        new_mode = write_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "252") == 0 || strcmp(new_permission, "0252") == 0) {
        new_mode = write_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "253") == 0 || strcmp(new_permission, "0253") == 0) {
        new_mode = write_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "254") == 0 || strcmp(new_permission, "0254") == 0) {
        new_mode = write_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "255") == 0 || strcmp(new_permission, "0255") == 0) {
        new_mode = write_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "256") == 0 || strcmp(new_permission, "0256") == 0) {
        new_mode = write_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "257") == 0 || strcmp(new_permission, "0257") == 0) {
        new_mode = write_user | read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "260") == 0 || strcmp(new_permission, "0260") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "261") == 0 || strcmp(new_permission, "0261") == 0) {
        new_mode = write_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "262") == 0 || strcmp(new_permission, "0262") == 0) {
        new_mode = write_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "263") == 0 || strcmp(new_permission, "0263") == 0) {
        new_mode = write_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "264") == 0 || strcmp(new_permission, "0264") == 0) {
        new_mode = write_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "265") == 0 || strcmp(new_permission, "0265") == 0) {
        new_mode = write_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "266") == 0 || strcmp(new_permission, "0266") == 0) {
        new_mode = write_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "267") == 0 || strcmp(new_permission, "0267") == 0) {
        new_mode = write_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "270") == 0 || strcmp(new_permission, "0270") == 0) {
        new_mode = write_user;
    } else if (strcmp(new_permission, "271") == 0 || strcmp(new_permission, "0271") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "272") == 0 || strcmp(new_permission, "0272") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "273") == 0 || strcmp(new_permission, "0273") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "274") == 0 || strcmp(new_permission, "0274") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "275") == 0 || strcmp(new_permission, "0275") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "276") == 0 || strcmp(new_permission, "0276") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "277") == 0 || strcmp(new_permission, "0277") == 0) {
        new_mode = write_user | read_group | write_group | exec_group | read_other |write_other | exec_other;      
    }

    // ====================================================================================

    else if (strcmp(new_permission, "300") == 0 || strcmp(new_permission, "0300") == 0) {
        new_mode =write_user | exec_user;
    } else if (strcmp(new_permission, "301") == 0 || strcmp(new_permission, "0301") == 0) {
        new_mode = write_user | exec_user | exec_other;
    } else if (strcmp(new_permission, "302") == 0 || strcmp(new_permission, "0302") == 0) {
        new_mode = write_user | exec_user | write_other;
    } else if (strcmp(new_permission, "303") == 0 || strcmp(new_permission, "0303") == 0) {
        new_mode = write_user | exec_user | write_other | exec_other;
    } else if (strcmp(new_permission, "304") == 0 || strcmp(new_permission, "0304") == 0) {
        new_mode = write_user | exec_user | read_other;
    } else if (strcmp(new_permission, "305") == 0 || strcmp(new_permission, "0305") == 0) {
        new_mode = write_user | exec_user | read_other | exec_other;
    } else if (strcmp(new_permission, "306") == 0 || strcmp(new_permission, "0306") == 0) {
        new_mode = write_user | exec_user | read_other | write_other;
    } else if (strcmp(new_permission, "307") == 0 || strcmp(new_permission, "0307") == 0) {
        new_mode = write_user | exec_user | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "310") == 0 || strcmp(new_permission, "0310") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "311") == 0 || strcmp(new_permission, "0311") == 0) {
        new_mode = write_user | exec_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "312") == 0 || strcmp(new_permission, "0312") == 0) {
        new_mode = write_user | exec_user | exec_group | write_other;
    } else if (strcmp(new_permission, "313") == 0 || strcmp(new_permission, "0313") == 0) {
        new_mode = write_user | exec_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "314") == 0 || strcmp(new_permission, "0314") == 0) {
        new_mode =write_user | exec_user |  exec_group | read_other;
    } else if (strcmp(new_permission, "315") == 0 || strcmp(new_permission, "0315") == 0) {
        new_mode =write_user | exec_user |  exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "316") == 0 || strcmp(new_permission, "0316") == 0) {
        new_mode =write_user | exec_user |  exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "317") == 0 || strcmp(new_permission, "0317") == 0) {
        new_mode =write_user | exec_user |  exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "320") == 0 || strcmp(new_permission, "0320") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "321") == 0 || strcmp(new_permission, "0321") == 0) {
        new_mode = write_user | exec_user | write_group | exec_other;
    } else if (strcmp(new_permission, "322") == 0 || strcmp(new_permission, "0322") == 0) {
        new_mode = write_user | exec_user | write_group | write_other;
    } else if (strcmp(new_permission, "323") == 0 || strcmp(new_permission, "0323") == 0) {
        new_mode = write_user | exec_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "324") == 0 || strcmp(new_permission, "0324") == 0) {
        new_mode = write_user | exec_user | write_group | read_other;
    } else if (strcmp(new_permission, "325") == 0 || strcmp(new_permission, "0325") == 0) {
        new_mode = write_user | exec_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "326") == 0 || strcmp(new_permission, "0326") == 0) {
        new_mode = write_user | exec_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "327") == 0 || strcmp(new_permission, "0327") == 0) {
        new_mode = write_user | exec_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "330") == 0 || strcmp(new_permission, "0330") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "331") == 0 || strcmp(new_permission, "0331") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "332") == 0 || strcmp(new_permission, "0332") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "333") == 0 || strcmp(new_permission, "0333") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "334") == 0 || strcmp(new_permission, "0334") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "335") == 0 || strcmp(new_permission, "0335") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "336") == 0 || strcmp(new_permission, "0336") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "337") == 0 || strcmp(new_permission, "0337") == 0) {
        new_mode = write_user | exec_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "340") == 0 || strcmp(new_permission, "0340") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "341") == 0 || strcmp(new_permission, "0341") == 0) {
        new_mode = write_user | exec_user | read_group | exec_other;
    } else if (strcmp(new_permission, "342") == 0 || strcmp(new_permission, "0342") == 0) {
        new_mode = write_user | exec_user | read_group | write_other;
    } else if (strcmp(new_permission, "343") == 0 || strcmp(new_permission, "0343") == 0) {
        new_mode = write_user | exec_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "344") == 0 || strcmp(new_permission, "0344") == 0) {
        new_mode =write_user | exec_user |  read_group | read_other;
    } else if (strcmp(new_permission, "345") == 0 || strcmp(new_permission, "0345") == 0) {
        new_mode =write_user | exec_user |  read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "346") == 0 || strcmp(new_permission, "0346") == 0) {
        new_mode =write_user | exec_user |  read_group | read_other | write_other;
    } else if (strcmp(new_permission, "347") == 0 || strcmp(new_permission, "0347") == 0) {
        new_mode =write_user | exec_user |  read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "350") == 0 || strcmp(new_permission, "0350") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "351") == 0 || strcmp(new_permission, "0351") == 0) {
        new_mode = write_user | exec_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "352") == 0 || strcmp(new_permission, "0352") == 0) {
        new_mode = read_group | write_user | exec_user | exec_group | write_other;
    } else if (strcmp(new_permission, "353") == 0 || strcmp(new_permission, "0353") == 0) {
        new_mode = read_group | write_user | exec_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "354") == 0 || strcmp(new_permission, "0354") == 0) {
        new_mode = read_group |write_user | exec_user |  exec_group | read_other;
    } else if (strcmp(new_permission, "355") == 0 || strcmp(new_permission, "0355") == 0) {
        new_mode = read_group |write_user | exec_user |  exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "356") == 0 || strcmp(new_permission, "0356") == 0) {
        new_mode = read_group |write_user | exec_user |  exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "357") == 0 || strcmp(new_permission, "0357") == 0) {
        new_mode = read_group |write_user | exec_user |  exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "360") == 0 || strcmp(new_permission, "0360") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "361") == 0 || strcmp(new_permission, "0361") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "362") == 0 || strcmp(new_permission, "0362") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "363") == 0 || strcmp(new_permission, "0363") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "364") == 0 || strcmp(new_permission, "0364") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "365") == 0 || strcmp(new_permission, "0365") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "366") == 0 || strcmp(new_permission, "0366") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "367") == 0 || strcmp(new_permission, "0367") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "370") == 0 || strcmp(new_permission, "0370") == 0) {
        new_mode = write_user | exec_user;
    } else if (strcmp(new_permission, "371") == 0 || strcmp(new_permission, "0371") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "372") == 0 || strcmp(new_permission, "0372") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "373") == 0 || strcmp(new_permission, "0373") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "374") == 0 || strcmp(new_permission, "0374") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "375") == 0 || strcmp(new_permission, "0375") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "376") == 0 || strcmp(new_permission, "0376") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "377") == 0 || strcmp(new_permission, "0377") == 0) {
        new_mode = write_user | exec_user | read_group | write_group | exec_group | read_other |write_other | exec_other;
    }

    // =====================

    else if (strcmp(new_permission, "400") == 0 || strcmp(new_permission, "0400") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "401") == 0 || strcmp(new_permission, "0401") == 0) {
        new_mode = read_user | exec_other;
    } else if (strcmp(new_permission, "402") == 0 || strcmp(new_permission, "0402") == 0) {
        new_mode = read_user | write_other;
    } else if (strcmp(new_permission, "403") == 0 || strcmp(new_permission, "0403") == 0) {
        new_mode = read_user | write_other | exec_other;
    } else if (strcmp(new_permission, "404") == 0 || strcmp(new_permission, "0404") == 0) {
        new_mode =read_user |  read_other;
    } else if (strcmp(new_permission, "405") == 0 || strcmp(new_permission, "0405") == 0) {
        new_mode =read_user |  read_other | exec_other;
    } else if (strcmp(new_permission, "406") == 0 || strcmp(new_permission, "0406") == 0) {
        new_mode =read_user |  read_other | write_other;
    } else if (strcmp(new_permission, "407") == 0 || strcmp(new_permission, "0407") == 0) {
        new_mode =read_user |  read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "410") == 0 || strcmp(new_permission, "0410") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "411") == 0 || strcmp(new_permission, "0411") == 0) {
        new_mode = read_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "412") == 0 || strcmp(new_permission, "0412") == 0) {
        new_mode = read_user | exec_group | write_other;
    } else if (strcmp(new_permission, "413") == 0 || strcmp(new_permission, "0413") == 0) {
        new_mode = read_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "414") == 0 || strcmp(new_permission, "0414") == 0) {
        new_mode = read_user | exec_group | read_other;
    } else if (strcmp(new_permission, "415") == 0 || strcmp(new_permission, "0415") == 0) {
        new_mode = read_user | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "416") == 0 || strcmp(new_permission, "0416") == 0) {
        new_mode = read_user | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "417") == 0 || strcmp(new_permission, "0417") == 0) {
        new_mode = read_user | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "420") == 0 || strcmp(new_permission, "0420") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "421") == 0 || strcmp(new_permission, "0421") == 0) {
        new_mode = read_user | write_group | exec_other;
    } else if (strcmp(new_permission, "422") == 0 || strcmp(new_permission, "0422") == 0) {
        new_mode = read_user | write_group | write_other;
    } else if (strcmp(new_permission, "423") == 0 || strcmp(new_permission, "0423") == 0) {
        new_mode = read_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "424") == 0 || strcmp(new_permission, "0424") == 0) {
        new_mode = read_user | write_group | read_other;
    } else if (strcmp(new_permission, "425") == 0 || strcmp(new_permission, "0425") == 0) {
        new_mode = read_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "426") == 0 || strcmp(new_permission, "0426") == 0) {
        new_mode = read_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "427") == 0 || strcmp(new_permission, "0427") == 0) {
        new_mode = read_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "430") == 0 || strcmp(new_permission, "0430") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "431") == 0 || strcmp(new_permission, "0431") == 0) {
        new_mode = read_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "432") == 0 || strcmp(new_permission, "0432") == 0) {
        new_mode = read_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "433") == 0 || strcmp(new_permission, "0433") == 0) {
        new_mode = read_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "434") == 0 || strcmp(new_permission, "0434") == 0) {
        new_mode = read_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "435") == 0 || strcmp(new_permission, "0435") == 0) {
        new_mode = read_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "436") == 0 || strcmp(new_permission, "0436") == 0) {
        new_mode = read_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "437") == 0 || strcmp(new_permission, "0437") == 0) {
        new_mode = read_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "440") == 0 || strcmp(new_permission, "0440") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "441") == 0 || strcmp(new_permission, "0441") == 0) {
        new_mode = read_user | read_group | exec_other;
    } else if (strcmp(new_permission, "442") == 0 || strcmp(new_permission, "0442") == 0) {
        new_mode = read_user | read_group | write_other;
    } else if (strcmp(new_permission, "443") == 0 || strcmp(new_permission, "0443") == 0) {
        new_mode = read_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "444") == 0 || strcmp(new_permission, "0444") == 0) {
        new_mode = read_user | read_group | read_other;
    } else if (strcmp(new_permission, "445") == 0 || strcmp(new_permission, "0445") == 0) {
        new_mode = read_user | read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "446") == 0 || strcmp(new_permission, "0446") == 0) {
        new_mode = read_user | read_group | read_other | write_other;
    } else if (strcmp(new_permission, "447") == 0 || strcmp(new_permission, "0447") == 0) {
        new_mode = read_user | read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "450") == 0 || strcmp(new_permission, "0450") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "451") == 0 || strcmp(new_permission, "0451") == 0) {
        new_mode = read_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "452") == 0 || strcmp(new_permission, "0452") == 0) {
        new_mode = read_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "453") == 0 || strcmp(new_permission, "0453") == 0) {
        new_mode = read_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "454") == 0 || strcmp(new_permission, "0454") == 0) {
        new_mode = read_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "455") == 0 || strcmp(new_permission, "0455") == 0) {
        new_mode = read_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "456") == 0 || strcmp(new_permission, "0456") == 0) {
        new_mode = read_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "457") == 0 || strcmp(new_permission, "0457") == 0) {
        new_mode = read_user | read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "460") == 0 || strcmp(new_permission, "0460") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "461") == 0 || strcmp(new_permission, "0461") == 0) {
        new_mode = read_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "462") == 0 || strcmp(new_permission, "0462") == 0) {
        new_mode = read_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "463") == 0 || strcmp(new_permission, "0463") == 0) {
        new_mode = read_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "464") == 0 || strcmp(new_permission, "0464") == 0) {
        new_mode = read_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "465") == 0 || strcmp(new_permission, "0465") == 0) {
        new_mode = read_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "466") == 0 || strcmp(new_permission, "0466") == 0) {
        new_mode = read_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "467") == 0 || strcmp(new_permission, "0467") == 0) {
        new_mode = read_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "470") == 0 || strcmp(new_permission, "0470") == 0) {
        new_mode = read_user;
    } else if (strcmp(new_permission, "471") == 0 || strcmp(new_permission, "0471") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "472") == 0 || strcmp(new_permission, "0472") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "473") == 0 || strcmp(new_permission, "0473") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "474") == 0 || strcmp(new_permission, "0474") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "475") == 0 || strcmp(new_permission, "0475") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "476") == 0 || strcmp(new_permission, "0476") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "477") == 0 || strcmp(new_permission, "0477") == 0) {
        new_mode = read_user | read_group | write_group | exec_group | read_other |write_other | exec_other;
    }


    // =========================

    else if (strcmp(new_permission, "500") == 0 || strcmp(new_permission, "0500") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "501") == 0 || strcmp(new_permission, "0501") == 0) {
        new_mode = read_user | exec_user | exec_other;
    } else if (strcmp(new_permission, "502") == 0 || strcmp(new_permission, "0502") == 0) {
        new_mode = read_user | exec_user | write_other;
    } else if (strcmp(new_permission, "503") == 0 || strcmp(new_permission, "0503") == 0) {
        new_mode = read_user | exec_user | write_other | exec_other;
    } else if (strcmp(new_permission, "504") == 0 || strcmp(new_permission, "0504") == 0) {
        new_mode = read_user | exec_user | read_other;
    } else if (strcmp(new_permission, "505") == 0 || strcmp(new_permission, "0505") == 0) {
        new_mode = read_user | exec_user | read_other | exec_other;
    } else if (strcmp(new_permission, "506") == 0 || strcmp(new_permission, "0506") == 0) {
        new_mode = read_user | exec_user | read_other | write_other;
    } else if (strcmp(new_permission, "507") == 0 || strcmp(new_permission, "0507") == 0) {
        new_mode = read_user | exec_user | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "510") == 0 || strcmp(new_permission, "0510") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "511") == 0 || strcmp(new_permission, "0511") == 0) {
        new_mode = read_user | exec_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "512") == 0 || strcmp(new_permission, "0512") == 0) {
        new_mode = read_user | exec_user | exec_group | write_other;
    } else if (strcmp(new_permission, "513") == 0 || strcmp(new_permission, "0513") == 0) {
        new_mode = read_user | exec_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "514") == 0 || strcmp(new_permission, "0514") == 0) {
        new_mode = read_user | exec_user | exec_group | read_other;
    } else if (strcmp(new_permission, "515") == 0 || strcmp(new_permission, "0515") == 0) {
        new_mode = read_user | exec_user | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "516") == 0 || strcmp(new_permission, "0516") == 0) {
        new_mode = read_user | exec_user | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "517") == 0 || strcmp(new_permission, "0517") == 0) {
        new_mode = read_user | exec_user | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "520") == 0 || strcmp(new_permission, "0520") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "521") == 0 || strcmp(new_permission, "0521") == 0) {
        new_mode = read_user | exec_user | write_group | exec_other;
    } else if (strcmp(new_permission, "522") == 0 || strcmp(new_permission, "0522") == 0) {
        new_mode = read_user | exec_user | write_group | write_other;
    } else if (strcmp(new_permission, "523") == 0 || strcmp(new_permission, "0523") == 0) {
        new_mode = read_user | exec_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "524") == 0 || strcmp(new_permission, "0524") == 0) {
        new_mode = read_user | exec_user | write_group | read_other;
    } else if (strcmp(new_permission, "525") == 0 || strcmp(new_permission, "0525") == 0) {
        new_mode = read_user | exec_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "526") == 0 || strcmp(new_permission, "0526") == 0) {
        new_mode = read_user | exec_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "527") == 0 || strcmp(new_permission, "0527") == 0) {
        new_mode = read_user | exec_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "530") == 0 || strcmp(new_permission, "0530") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "531") == 0 || strcmp(new_permission, "0531") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "532") == 0 || strcmp(new_permission, "0532") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "533") == 0 || strcmp(new_permission, "0533") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "534") == 0 || strcmp(new_permission, "0534") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "535") == 0 || strcmp(new_permission, "0535") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "536") == 0 || strcmp(new_permission, "0536") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "537") == 0 || strcmp(new_permission, "0537") == 0) {
        new_mode = read_user | exec_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "540") == 0 || strcmp(new_permission, "0540") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "541") == 0 || strcmp(new_permission, "0541") == 0) {
        new_mode = read_user | exec_user | read_group | exec_other;
    } else if (strcmp(new_permission, "542") == 0 || strcmp(new_permission, "0542") == 0) {
        new_mode = read_user | exec_user | read_group | write_other;
    } else if (strcmp(new_permission, "543") == 0 || strcmp(new_permission, "0543") == 0) {
        new_mode = read_user | exec_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "544") == 0 || strcmp(new_permission, "0544") == 0) {
        new_mode = read_user | exec_user | read_group | read_other;
    } else if (strcmp(new_permission, "545") == 0 || strcmp(new_permission, "0545") == 0) {
        new_mode = read_user | exec_user | read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "546") == 0 || strcmp(new_permission, "0546") == 0) {
        new_mode = read_user | exec_user | read_group | read_other | write_other;
    } else if (strcmp(new_permission, "547") == 0 || strcmp(new_permission, "0547") == 0) {
        new_mode = read_user | exec_user | read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "550") == 0 || strcmp(new_permission, "0550") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "551") == 0 || strcmp(new_permission, "0551") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "552") == 0 || strcmp(new_permission, "0552") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "553") == 0 || strcmp(new_permission, "0553") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "554") == 0 || strcmp(new_permission, "0554") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "555") == 0 || strcmp(new_permission, "0555") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "556") == 0 || strcmp(new_permission, "0556") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "557") == 0 || strcmp(new_permission, "0557") == 0) {
        new_mode = read_user | exec_user | read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "560") == 0 || strcmp(new_permission, "0560") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "561") == 0 || strcmp(new_permission, "0561") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "562") == 0 || strcmp(new_permission, "0562") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "563") == 0 || strcmp(new_permission, "0563") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "564") == 0 || strcmp(new_permission, "0564") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "565") == 0 || strcmp(new_permission, "0565") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "566") == 0 || strcmp(new_permission, "0566") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "567") == 0 || strcmp(new_permission, "0567") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "570") == 0 || strcmp(new_permission, "0570") == 0) {
        new_mode = read_user | exec_user;
    } else if (strcmp(new_permission, "571") == 0 || strcmp(new_permission, "0571") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "572") == 0 || strcmp(new_permission, "0572") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "573") == 0 || strcmp(new_permission, "0573") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "574") == 0 || strcmp(new_permission, "0574") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "575") == 0 || strcmp(new_permission, "0575") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "576") == 0 || strcmp(new_permission, "0576") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "577") == 0 || strcmp(new_permission, "0577") == 0) {
        new_mode = read_user | exec_user | read_group | write_group | exec_group | read_other |write_other | exec_other;
    }

    // ====================================================================================

    else if (strcmp(new_permission, "600") == 0 || strcmp(new_permission, "0600") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "601") == 0 || strcmp(new_permission, "0601") == 0) {
        new_mode = read_user | write_user | exec_other;
    } else if (strcmp(new_permission, "602") == 0 || strcmp(new_permission, "0602") == 0) {
        new_mode = read_user | write_user | write_other;
    } else if (strcmp(new_permission, "603") == 0 || strcmp(new_permission, "0603") == 0) {
        new_mode = read_user | write_user | write_other | exec_other;
    } else if (strcmp(new_permission, "604") == 0 || strcmp(new_permission, "0604") == 0) {
        new_mode = read_user | write_user | read_other;
    } else if (strcmp(new_permission, "605") == 0 || strcmp(new_permission, "0605") == 0) {
        new_mode = read_user | write_user | read_other | exec_other;
    } else if (strcmp(new_permission, "606") == 0 || strcmp(new_permission, "0606") == 0) {
        new_mode = read_user | write_user | read_other | write_other;
    } else if (strcmp(new_permission, "607") == 0 || strcmp(new_permission, "0607") == 0) {
        new_mode = read_user | write_user | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "610") == 0 || strcmp(new_permission, "0610") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "611") == 0 || strcmp(new_permission, "0611") == 0) {
        new_mode = read_user | write_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "612") == 0 || strcmp(new_permission, "0612") == 0) {
        new_mode = read_user | write_user | exec_group | write_other;
    } else if (strcmp(new_permission, "613") == 0 || strcmp(new_permission, "0613") == 0) {
        new_mode = read_user | write_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "614") == 0 || strcmp(new_permission, "0614") == 0) {
        new_mode =read_user | write_user |  exec_group | read_other;
    } else if (strcmp(new_permission, "615") == 0 || strcmp(new_permission, "0615") == 0) {
        new_mode =read_user | write_user |  exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "616") == 0 || strcmp(new_permission, "0616") == 0) {
        new_mode =read_user | write_user |  exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "617") == 0 || strcmp(new_permission, "0617") == 0) {
        new_mode =read_user | write_user |  exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "620") == 0 || strcmp(new_permission, "0620") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "621") == 0 || strcmp(new_permission, "0621") == 0) {
        new_mode = read_user | write_user | write_group | exec_other;
    } else if (strcmp(new_permission, "622") == 0 || strcmp(new_permission, "0622") == 0) {
        new_mode = read_user | write_user | write_group | write_other;
    } else if (strcmp(new_permission, "623") == 0 || strcmp(new_permission, "0623") == 0) {
        new_mode = read_user | write_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "624") == 0 || strcmp(new_permission, "0624") == 0) {
        new_mode = read_user | write_user | write_group | read_other;
    } else if (strcmp(new_permission, "625") == 0 || strcmp(new_permission, "0625") == 0) {
        new_mode = read_user | write_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "626") == 0 || strcmp(new_permission, "0626") == 0) {
        new_mode = read_user | write_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "627") == 0 || strcmp(new_permission, "0627") == 0) {
        new_mode = read_user | write_user | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "630") == 0 || strcmp(new_permission, "0630") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "631") == 0 || strcmp(new_permission, "0631") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "632") == 0 || strcmp(new_permission, "0632") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "633") == 0 || strcmp(new_permission, "0633") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "634") == 0 || strcmp(new_permission, "0634") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "635") == 0 || strcmp(new_permission, "0635") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "636") == 0 || strcmp(new_permission, "0636") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "637") == 0 || strcmp(new_permission, "0637") == 0) {
        new_mode = read_user | write_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "640") == 0 || strcmp(new_permission, "0640") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "641") == 0 || strcmp(new_permission, "0641") == 0) {
        new_mode = read_user | write_user | read_group | exec_other;
    } else if (strcmp(new_permission, "642") == 0 || strcmp(new_permission, "0642") == 0) {
        new_mode = read_user | write_user | read_group | write_other;
    } else if (strcmp(new_permission, "643") == 0 || strcmp(new_permission, "0643") == 0) {
        new_mode = read_user | write_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "644") == 0 || strcmp(new_permission, "0644") == 0) {
        new_mode =read_user | write_user |  read_group | read_other;
    } else if (strcmp(new_permission, "645") == 0 || strcmp(new_permission, "0645") == 0) {
        new_mode =read_user | write_user |  read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "646") == 0 || strcmp(new_permission, "0646") == 0) {
        new_mode =read_user | write_user |  read_group | read_other | write_other;
    } else if (strcmp(new_permission, "647") == 0 || strcmp(new_permission, "0647") == 0) {
        new_mode =read_user | write_user |  read_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "650") == 0 || strcmp(new_permission, "0650") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "651") == 0 || strcmp(new_permission, "0651") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "652") == 0 || strcmp(new_permission, "0652") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "653") == 0 || strcmp(new_permission, "0653") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "654") == 0 || strcmp(new_permission, "0654") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "655") == 0 || strcmp(new_permission, "0655") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "656") == 0 || strcmp(new_permission, "0656") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "657") == 0 || strcmp(new_permission, "0657") == 0) {
        new_mode = read_user | write_user | read_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "660") == 0 || strcmp(new_permission, "0660") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "661") == 0 || strcmp(new_permission, "0661") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "662") == 0 || strcmp(new_permission, "0662") == 0) {
        new_mode = read_user | write_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "663") == 0 || strcmp(new_permission, "0663") == 0) {
        new_mode = read_user | write_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "664") == 0 || strcmp(new_permission, "0664") == 0) {
        new_mode = read_user | write_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "665") == 0 || strcmp(new_permission, "0665") == 0) {
        new_mode = read_user | write_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "666") == 0 || strcmp(new_permission, "0666") == 0) {
        new_mode = read_user | write_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "667") == 0 || strcmp(new_permission, "0667") == 0) {
        new_mode = read_user | write_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "670") == 0 || strcmp(new_permission, "0670") == 0) {
        new_mode = read_user | write_user;
    } else if (strcmp(new_permission, "671") == 0 || strcmp(new_permission, "0671") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "672") == 0 || strcmp(new_permission, "0672") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "673") == 0 || strcmp(new_permission, "0673") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "674") == 0 || strcmp(new_permission, "0674") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "675") == 0 || strcmp(new_permission, "0675") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "676") == 0 || strcmp(new_permission, "0676") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "677") == 0 || strcmp(new_permission, "0677") == 0) {
        new_mode = read_user | write_user | read_group | write_group | exec_group | read_other |write_other | exec_other;
    }
    
    // ====================================================================================
    
    
    else if (strcmp(new_permission, "700") == 0 || strcmp(new_permission, "0700") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "701") == 0 || strcmp(new_permission, "0701") == 0) {
        new_mode = read_user | write_user | exec_user | exec_other;
    } else if (strcmp(new_permission, "702") == 0 || strcmp(new_permission, "0702") == 0) {
        new_mode = read_user | write_user | exec_user | write_other;
    } else if (strcmp(new_permission, "703") == 0 || strcmp(new_permission, "0703") == 0) {
        new_mode = read_user | write_user | exec_user | write_other | exec_other;
    } else if (strcmp(new_permission, "704") == 0 || strcmp(new_permission, "0704") == 0) {
        new_mode = read_user | write_user | exec_user | read_other;
    } else if (strcmp(new_permission, "705") == 0 || strcmp(new_permission, "0705") == 0) {
        new_mode = read_user | write_user | exec_user | read_other | exec_other;
    } else if (strcmp(new_permission, "706") == 0 || strcmp(new_permission, "0706") == 0) {
        new_mode = read_user | write_user | exec_user | read_other | write_other;
    } else if (strcmp(new_permission, "707") == 0 || strcmp(new_permission, "0707") == 0) {
        new_mode = read_user | write_user | exec_user | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "710") == 0 || strcmp(new_permission, "0710") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "711") == 0 || strcmp(new_permission, "0711") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | exec_other;
    } else if (strcmp(new_permission, "712") == 0 || strcmp(new_permission, "0712") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | write_other;
    } else if (strcmp(new_permission, "713") == 0 || strcmp(new_permission, "0713") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "714") == 0 || strcmp(new_permission, "0714") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | read_other;
    } else if (strcmp(new_permission, "715") == 0 || strcmp(new_permission, "0715") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "716") == 0 || strcmp(new_permission, "0716") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "717") == 0 || strcmp(new_permission, "0717") == 0) {
        new_mode = read_user | write_user | exec_user | exec_group | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "720") == 0 || strcmp(new_permission, "0720") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "721") == 0 || strcmp(new_permission, "0721") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_other;
    } else if (strcmp(new_permission, "722") == 0 || strcmp(new_permission, "0722") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | write_other;
    } else if (strcmp(new_permission, "723") == 0 || strcmp(new_permission, "0723") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "724") == 0 || strcmp(new_permission, "0724") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | read_other;
    } else if (strcmp(new_permission, "725") == 0 || strcmp(new_permission, "0725") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "726") == 0 || strcmp(new_permission, "0726") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "727") == 0 || strcmp(new_permission, "0727") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "730") == 0 || strcmp(new_permission, "0730") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "731") == 0 || strcmp(new_permission, "0731") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "732") == 0 || strcmp(new_permission, "0732") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "733") == 0 || strcmp(new_permission, "0733") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "734") == 0 || strcmp(new_permission, "0734") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "735") == 0 || strcmp(new_permission, "0735") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "736") == 0 || strcmp(new_permission, "0736") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "737") == 0 || strcmp(new_permission, "0737") == 0) {
        new_mode = read_user | write_user | exec_user | write_group | exec_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "740") == 0 || strcmp(new_permission, "0740") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "741") == 0 || strcmp(new_permission, "0741") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_other;
    } else if (strcmp(new_permission, "742") == 0 || strcmp(new_permission, "0742") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_other;
    } else if (strcmp(new_permission, "743") == 0 || strcmp(new_permission, "0743") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_other | exec_other;
    } else if (strcmp(new_permission, "744") == 0 || strcmp(new_permission, "0744") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | read_other;
    } else if (strcmp(new_permission, "745") == 0 || strcmp(new_permission, "0745") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | read_other | exec_other;
    } else if (strcmp(new_permission, "746") == 0 || strcmp(new_permission, "0746") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | read_other | write_other;
    } else if (strcmp(new_permission, "747") == 0 || strcmp(new_permission, "0747") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "750") == 0 || strcmp(new_permission, "0750") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "751") == 0 || strcmp(new_permission, "0751") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "752") == 0 || strcmp(new_permission, "0752") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | write_other;
    } else if (strcmp(new_permission, "753") == 0 || strcmp(new_permission, "0753") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "754") == 0 || strcmp(new_permission, "0754") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | read_other;
    } else if (strcmp(new_permission, "755") == 0 || strcmp(new_permission, "0755") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "756") == 0 || strcmp(new_permission, "0756") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "757") == 0 || strcmp(new_permission, "0757") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | exec_group | read_other | write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "760") == 0 || strcmp(new_permission, "0760") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "761") == 0 || strcmp(new_permission, "0761") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_other;
    } else if (strcmp(new_permission, "762") == 0 || strcmp(new_permission, "0762") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | write_other;
    } else if (strcmp(new_permission, "763") == 0 || strcmp(new_permission, "0763") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | write_other | exec_other;
    } else if (strcmp(new_permission, "764") == 0 || strcmp(new_permission, "0764") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | read_other;
    } else if (strcmp(new_permission, "765") == 0 || strcmp(new_permission, "0765") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | read_other | exec_other;
    } else if (strcmp(new_permission, "766") == 0 || strcmp(new_permission, "0766") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | read_other | write_other;
    } else if (strcmp(new_permission, "767") == 0 || strcmp(new_permission, "0767") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | read_other |write_other | exec_other;
    } 
    
    else if (strcmp(new_permission, "770") == 0 || strcmp(new_permission, "0770") == 0) {
        new_mode = read_user | write_user | exec_user;
    } else if (strcmp(new_permission, "771") == 0 || strcmp(new_permission, "0771") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | exec_other;
    } else if (strcmp(new_permission, "772") == 0 || strcmp(new_permission, "0772") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | write_other;
    } else if (strcmp(new_permission, "773") == 0 || strcmp(new_permission, "0773") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | write_other | exec_other;
    } else if (strcmp(new_permission, "774") == 0 || strcmp(new_permission, "0774") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | read_other;
    } else if (strcmp(new_permission, "775") == 0 || strcmp(new_permission, "0775") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | read_other | exec_other;
    } else if (strcmp(new_permission, "776") == 0 || strcmp(new_permission, "0776") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | read_other | write_other;
    } else if (strcmp(new_permission, "777") == 0 || strcmp(new_permission, "0777") == 0) {
        new_mode = read_user | write_user | exec_user | read_group | write_group | exec_group | read_other | write_other | exec_other;
    }

    // ===================================================================================

    return new_mode;
}

