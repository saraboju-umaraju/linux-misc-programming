#include <linux/init.h>
#include </usr/src/linux-4.13.2+/fs/ext4/ext4.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/path.h>
#include <linux/list.h>
#include <linux/spinlock.h>


char *str_tok(char * p1, char *p2)
{
        static int z;
        static char *s;
        int start = z;
        if(p1!=NULL)
                s = p1;

        int j = 0;
        while(s[z] != '\0')
        {
                j = 0;
                while(p2[j] != '\0')
                {
                        if(s[z] == p2[j])
                        {
                                s[z] = '\0';
                                z = z+1;
                                if(s[start] != '\0')
                                        return (&s[start]);
                                else
                                {

                                        start = z;
                                        z--;
                                        break;
                                }
                        }
                        j++;
                }
                z++;
        }
        s[z] = '\0';
        if(s[start] == '\0')
                return NULL;
        else
                return &s[start];
}



char path[] = "Downloads/umas_forte/.eGo/zourneY/umasraZ/strings/strfuns";

void parse_item(struct dentry*,int );

extern int ext4_get_inode_loc(struct inode *, struct ext4_iloc *);

struct dentry *get_dentry(struct dentry *parent, char *str)
{
	struct dentry *child;
	spin_lock(&parent->d_lock);
	list_for_each_entry(child, &parent->d_subdirs, d_child){
		spin_lock(&child->d_lock);
			if (!strcmp(str,child->d_name.name)){
				spin_unlock(&child->d_lock);
				spin_unlock(&parent->d_lock);
				return child;
			}
		spin_unlock(&child->d_lock);
	}
	
	spin_unlock(&parent->d_lock);
	return NULL;
}

struct dentry *test(void)
{

	struct dentry *parent = current->fs->pwd.dentry;
	char *ret = NULL;
        ret=str_tok(path,"/");
	while (ret!= NULL){
                printk ("the token is: %s\n",ret);
		parent = get_dentry(parent, ret);
		if (!parent)
			return NULL;
		printk ("%u\n",parent->d_inode->i_ino);
                ret = str_tok(NULL,"/");
        }

}

void parse_dir(struct dentry *parent,int val)
{

	printk("%*s DIRECTORY : %s\n",val,"", parent->d_name.name);

	struct dentry *child;
	list_for_each_entry(child, &parent->d_subdirs, d_child){
	if (!strcmp("saraboju",child->d_name.name)){
	}
		parse_item(child,val+1);

	}
}

void parse_file(struct dentry *dent,int val)
{
	printk("%*s FILE : %s ",val,"", dent->d_name.name);

	struct ext4_inode_info	*inode = EXT4_I(dent->d_inode);
/*
	if (inode)
		printk("\t\tdisk size %u\n",inode->i_disksize);
	else
		printk("\t\tinode is NULL\n");
*/
}

void parse_item(struct dentry *child,int val)
{
	spin_lock(&child->d_lock);
	

	if ( (0x00200000 & child->d_flags) && (0x00400000 & child->d_flags) == 0 ) {
		parse_dir(child,val+1);
	}

	else if ( 0x00400000 & child->d_flags ) {
		;//parse_file(child,val);
	}

	spin_unlock(&child->d_lock);

}


static int __init subdirstest_init(void)
{
	struct dentry *parent = current->fs->pwd.dentry;
	struct inode *parent99 = current->fs->pwd.dentry->d_inode;
	struct dentry *child;
	struct ext4_inode_info *inode = NULL;

	spin_lock(&parent->d_lock);

	list_for_each_entry(child, &parent->d_subdirs, d_child) {

		parse_item(child,0);
	}
	spin_unlock(&parent->d_lock);
	test();
	return 0;
}

static void __exit subdirstest_exit(void)
{
}

module_init(subdirstest_init);
module_exit(subdirstest_exit);
