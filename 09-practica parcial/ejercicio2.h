
struct multiconj;
typedef struct multiconj multiconj_t;

multiconj_t* multiconj_crear();
bool multiconj_guardar(multiconj_t* multiconj, char* elem);

bool multiconj_pertenece(multiconj_t* multiconj, char* elem);

bool multiconj_borrar(multiconj_t* multiconj, char* elem);