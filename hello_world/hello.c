/*
 * @Author: xiafeng.luo xiafeng.luo@hongjingdrive.com
 * @Date: 2022-06-07 17:20:10
 * @LastEditors: luoxiafeng luoxiafeng1990@163.com
 * @LastEditTime: 2022-06-16 14:31:15
 * @FilePath: /linux-5.3.7/drivers/examples/hello_world/hello.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kern_levels.h>
#include <linux/stddef.h>
#include <linux/errno.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiafeng.luo");
MODULE_DESCRIPTION("This is just a hello module!\n");


typedef struct imageData {
	int16_t width;
	int16_t height;
	int8_t color_space;
	int32_t frame_id;
	struct list_head list;
} image;

static struct list_head *create_list(int type)
{
	switch (type) {
	case 1: {
		static LIST_HEAD(img_list);
		return &img_list;
	} break;
	case 2: {
		static struct imageData imgdata = {
			.list = LIST_HEAD_INIT(imgdata.list),
		};
		return &imgdata.list;
	} break;
	default:;
	}
}

static void init_list_head(struct list_head *list)
{
	INIT_LIST_HEAD(list);
}

static void insert_list_at_head(struct list_head *new, struct list_head *head)
{
	list_add(new, head);
}
static void insert_list_at_tail(struct list_head *new, struct list_head *head)
{
	list_add_tail(new, head);
}
static void delete_list_element(struct list_head *element)
{
	list_del(element);
}
static void delete_full_list(struct list_head *head)
{
	image *tmp = NULL;
	image *tmpn = NULL;
	/*
        *(1)list,is struct member of "struct imageData" 
         */
	list_for_each_entry_safe (tmp, tmpn, head, list) {
             delete_list_element(&tmp->list);
             kfree(tmp);  
	}
}
static void is_list_empty(struct list_head* head) 
{
        if (list_empty(head))
                printk("list is empty\n"); 
        else
                printk("list is not empty!\n");
}

static void find_first_entry(struct list_head* head) 
{
        struct list_head* cur = NULL;
        struct imageData* image;
        list_for_each(cur, head) {
                if (list_is_first(cur,head)) {
                        image = list_entry(cur,struct imageData,list);
                        printk("first node-->image.frame_id = %d\n",image->frame_id);
                }
        }
}

static void find_last_entry(struct list_head* head)
{
        struct list_head* cur = NULL;
        struct imageData* image;
        list_for_each(cur, head) {
                if (list_is_last(cur,head)) {
                        image = list_entry(cur,struct imageData,list);
                        printk("last node-->image.frame_id = %d\n",image->frame_id);
                }
        }
}

static void iterate_display_list(struct list_head *head)
{
	image *tmp = NULL;
	image *tmpn = NULL;
	struct list_head *cur;
	struct list_head *n;
	/*
        *(1)list,is struct member of "struct imageData" 
         */
	printk("==============iterate start:\n");
	list_for_each (cur, head) {
		tmp = list_entry(cur, struct imageData, list);
		printk("tmp.frame_id = %d\n", tmp->frame_id);
	}
	printk("================================\n");
	list_for_each_entry (tmp, head, list) {
		printk("tmp.frame_id = %d\n", tmp->frame_id);
	}
	printk("================================\n");
	list_for_each_prev (cur, head) {
		tmp = list_entry(cur, struct imageData, list);
		printk("tmp.frame_id = %d\n", tmp->frame_id);
	}
	printk("================================\n");
	list_for_each_safe (cur, n, head) {
		tmp = list_entry(cur, struct imageData, list);
		printk("tmp.frame_id = %d\n", tmp->frame_id);
	}
	printk("================================\n");
	list_for_each_entry_safe (tmp, tmpn, head, list) {
		printk("tmp.frame_id = %d\n", tmp->frame_id);
	}
	printk("==============iterate over:\n");
}
static void test_summer(void)
{
        uint32_t a;
        size_t addr;
        if(__same_type(a,uint32_t))
                printk("uint32_t type variable\n");
        else
                printk("unknown type variable\n");
        image img = {
                .width = 1920,
                .height = 1280,
                .color_space = 1,
                .frame_id = 0,
                .list = LIST_HEAD_INIT(img.list),
        };
        addr = (size_t)&img.list - (size_t)&(((image*)0)->list);
        printk("calc addr:0x%zx,struct addr:0x%zx\n", addr,(size_t)&img);
}
static int __init hello_init(void)
{
	int i = 0;
	image *pimage = NULL;
	image *tmp = NULL;

	pimage = kmalloc(sizeof(*pimage), GFP_KERNEL);
	if (NULL == pimage) {
		printk("kmalloc error\n");
		return -1;
	}
	init_list_head(&pimage->list);
	for (i = 0; i < 10; i++) {
		tmp = kmalloc(sizeof(*pimage), GFP_KERNEL);
		init_list_head(&tmp->list);
		tmp->frame_id = i;
		insert_list_at_head(&tmp->list, &pimage->list);
	}
	iterate_display_list(&pimage->list);
        find_first_entry(&pimage->list);
        find_last_entry(&pimage->list);
        delete_full_list(&pimage->list);
        is_list_empty(&pimage->list);
        test_summer();

        kfree(pimage);
	printk(KERN_EMERG "hello, init\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_EMERG "hello, exit\n");
}

module_init(hello_init);
module_exit(hello_exit);