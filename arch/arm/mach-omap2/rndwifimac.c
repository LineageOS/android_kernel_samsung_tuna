/*
 * Author: Ryan Welton <Ryan.G.Weton@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/rndwifimac.h>

int randomize_wifi_mac;

/* sysfs interface */
static ssize_t randomize_wifi_mac_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", randomize_wifi_mac);
}

static ssize_t randomize_wifi_mac_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
sscanf(buf, "%du", &randomize_wifi_mac);
return count;
}

static struct kobj_attribute randomize_wifi_mac_attribute =
__ATTR(randomize_wifi_mac, 0666, randomize_wifi_mac_show, randomize_wifi_mac_store);

static struct attribute *attrs[] = {
&randomize_wifi_mac_attribute.attr,
NULL,
};

static struct attribute_group attr_group = {
.attrs = attrs,
};

static struct kobject *rnd_wifi_mac_kobj;

int rnd_wifi_mac_init(void)
{
	int retval;

	randomize_wifi_mac = 0;

        rnd_wifi_mac_kobj = kobject_create_and_add("rnd_wifi_mac", kernel_kobj);
        if (!rnd_wifi_mac_kobj) {
                return -ENOMEM;
        }
        retval = sysfs_create_group(rnd_wifi_mac_kobj, &attr_group);
        if (retval)
                kobject_put(rnd_wifi_mac_kobj);
        return retval;
}
/* end sysfs interface */

void rnd_wifi_mac_exit(void)
{
	kobject_put(rnd_wifi_mac_kobj);
}

module_init(rnd_wifi_mac_init);
module_exit(rnd_wifi_mac_exit);
