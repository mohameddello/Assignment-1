#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// بنية تمثل المخزن الدائري (Ring Buffer)
typedef struct {
    char *buffer;
    int max_size;
    int head;
    int tail;
} RingBuf;

// دالة لتهيئة المخزن وحجز الذاكرة
int init_ringbuf(RingBuf *rb, int capacity) {
    if (capacity <= 0) return 0;
    
    rb->buffer = (char *)malloc(capacity + 1);
    if (rb->buffer == NULL) return 0;
    
    rb->max_size = capacity;
    rb->head = 0;
    rb->tail = 0;
    return 1;
}

// تنظيف الذاكرة بعد الانتهاء
void free_ringbuf(RingBuf *rb) {
    if (rb->buffer != NULL) {
        free(rb->buffer);
        rb->buffer = NULL;
    }
}

// فحص إذا كان المخزن فارغاً
int is_empty(RingBuf *rb) {
    return (rb->head == rb->tail);
}

// فحص إذا كان المخزن ممتلئاً
int is_full(RingBuf *rb) {
    return (((rb->head + 1) % (rb->max_size + 1)) == rb->tail);
}

// إضافة (كتابة) عنصر داخل المخزن
int write_data(RingBuf *rb, char val) {
    if (is_full(rb)) return 0; // فشل الكتابة إذا كان ممتلئاً
    
    rb->buffer[rb->head] = val;
    rb->head = (rb->head + 1) % (rb->max_size + 1);
    return 1;
}

// سحب (قراءة) عنصر من المخزن
int read_data(RingBuf *rb, char *data_out) {
    if (is_empty(rb)) return 0; // فشل القراءة إذا كان فارغاً
    
    *data_out = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % (rb->max_size + 1);
    return 1;
}

int main() {
    char user_name[100];
    printf("الاسم: ");
    
    // قراءة الإدخال بطريقة آمنة
    if (fgets(user_name, sizeof(user_name), stdin) != NULL) {
        size_t len = strlen(user_name);
        // التخلص من سطر النهاية (Enter)
        if (len > 0 && user_name[len - 1] == '\n') {
            user_name[len - 1] = '\0';
        }
    }

    // دمج الاسم مع اللاحقة المطلوبة
    char final_str[150];
    sprintf(final_str, "%s CE-ESY", user_name);
    int total_len = strlen(final_str);

    printf("\nالناتج: %s (الطول=%d)\n\n", final_str, total_len);

    // --- الاختبار الأول: مخزن صغير ---
    int small_cap = total_len - 2;
    if (small_cap <= 0) small_cap = 1;

    RingBuf rb_small;
    init_ringbuf(&rb_small, small_cap);

    printf("حالة المخزن بحجم: %d:\n", small_cap);

    // محاولة إدخال النص بالكامل في المخزن الصغير
    for (int i = 0; i < total_len; i++) {
        if (write_data(&rb_small, final_str[i]) == 0) {
            printf("تعذرت كتابة الحرف '%c' (المخزن ممتلئ)\n", final_str[i]);
        }
    }

    printf("المخرجات المقروءة: ");
    char temp_char;
    while (read_data(&rb_small, &temp_char)) {
        printf("%c", temp_char);
    }
    printf("\n\n");
    free_ringbuf(&rb_small);

    // --- الاختبار الثاني: مخزن كبير يستوعب النص ---
    RingBuf rb_large;
    init_ringbuf(&rb_large, total_len + 5);

    int all_written = 1;
    printf("حالة المخزن بحجم %d:\n", total_len + 5);

    // إدخال النص في المخزن الكبير
    for (int i = 0; i < total_len; i++) {
        if (write_data(&rb_large, final_str[i]) == 0) {
            all_written = 0;
        }
    }

    printf("هل تمت كتابة النص بالكامل؟ %s\n", all_written ? "نعم" : "لا");

    printf("المخرجات المقروءة: ");
    while (read_data(&rb_large, &temp_char)) {
        printf("%c", temp_char);
    }
    printf("\n");

    free_ringbuf(&rb_large);
    return 0;
}

