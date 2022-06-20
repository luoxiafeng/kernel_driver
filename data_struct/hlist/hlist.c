/*
 * @Author: luoxiafeng luoxiafeng1990@163.com
 * @Date: 2022-06-16 14:28:24
 * @LastEditors: luoxiafeng luoxiafeng1990@163.com
 * @LastEditTime: 2022-06-20 17:54:34
 * @FilePath: /linux-5.3.7/drivers/examples/data_struct/hlist/hlist.c
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
#include <linux/stddef.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiafeng.luo");
MODULE_DESCRIPTION("This is just a hlist module!\n");

struct imageData {
        uint32_t width;
        uint32_t height;
        uint8_t  color_space;
        uint32_t frame_id;
        struct hlist_node hlist_node;
};

static void iterate_display_hlist(struct hlist_head* header)
{
        struct imageData* cur = NULL;
        /*
        *（1）hlist_node是结构体cur的成员 
        */
        printk("==============iterate start:\n");
        hlist_for_each_entry(cur,header,hlist_node) {
            printk("frame_id: %d\n",cur->frame_id); 
            kfree(cur);
        }
        printk("==============iterate over:\n");
}

static void delete_full_hlist(struct hlist_head* header)
{
        struct hlist_node* node = NULL;
        struct hlist_node* n;
        struct imageData* img = NULL;
        hlist_for_each_safe(node,n,header) {
            hlist_entry_safe(img,struct imageData,hlist_node);
            hlist_del(node);
            kfree(img);
        }
}

static void is_hlist_empty(struct hlist_head* head)
{
        if (hlist_empty(head)) {
            printk("hlist is empty\n");
        } else {
            printk("hlist is not empty\n");
        }
}

static int __init hlist_init(void) 
{
        uint32_t i = 0;
        struct imageData* tmp = NULL;
        HLIST_HEAD(hlist_header); 
        //add elements to hlist
        for (i = 0; i < 10; i ++ ) {
            tmp = kmalloc(sizeof(struct imageData),GFP_KERNEL);
            tmp->frame_id = i;
            hlist_add_head(&tmp->hlist_node,&hlist_header);
        }
        is_hlist_empty(&hlist_header);
        iterate_display_hlist(&hlist_header);
        delete_full_hlist(&hlist_header);
        is_hlist_empty(&hlist_header);
        printk("This is hlist_init \n");
        return 0;
}

static void __exit hlist_exit(void)
{
        printk("This is hlist_exit \n");
        return 0;
}

module_init(hlist_init);
module_exit(hlist_exit);
