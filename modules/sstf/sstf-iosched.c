/*
 * SSTF IO Scheduler
 *
 * For Kernel 4.13.9
 */

#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

/* SSTF data structure. */
struct sstf_data {
	struct list_head queue;  // List of requests
	sector_t head_pos;       // Current head position
};

/* Merging requests: remove the next request from the list. */
static void sstf_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

/* Dispatch the request with the shortest seek time (closest to head position). */
static int sstf_dispatch(struct request_queue *q, int force)
{
	struct sstf_data *nd = q->elevator->elevator_data;
	struct request *rq, *closest_rq = NULL;
	struct list_head *pos;
	sector_t closest_distance = ~((sector_t)0); // Maximum possible distance

	if (list_empty(&nd->queue))  // Check if queue is empty
		return 0;

	// Traverse the queue to find the request closest to the current head position
	list_for_each(pos, &nd->queue) {
		rq = list_entry(pos, struct request, queuelist);
		sector_t distance = abs(blk_rq_pos(rq) - nd->head_pos);

		if (distance < closest_distance) {
			closest_distance = distance;
			closest_rq = rq;
		}
	}

	if (closest_rq) {
		list_del_init(&closest_rq->queuelist);
		elv_dispatch_sort(q, closest_rq);
		nd->head_pos = blk_rq_pos(closest_rq);  // Update head position
		printk(KERN_INFO "[SSTF] Dispatching sector %llu (new head pos: %llu)\n",
		       blk_rq_pos(closest_rq), nd->head_pos);
		return 1;
	}

	return 0;
}

/* Add a request to the queue, maintaining SSTF order (shortest seek time first). */
static void sstf_add_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;
	struct list_head *pos;
	struct request *iter_rq;

	// Insert the new request in the queue, sorted by proximity to the head position
	list_for_each(pos, &nd->queue) {
		iter_rq = list_entry(pos, struct request, queuelist);
		if (abs(blk_rq_pos(iter_rq) - nd->head_pos) > abs(blk_rq_pos(rq) - nd->head_pos)) {
			list_add_tail(&rq->queuelist, pos);  // Insert the new request before iter_rq
			printk(KERN_INFO "[SSTF] Added sector %llu (current head pos: %llu)\n",
			       blk_rq_pos(rq), nd->head_pos);
			return;
		}
	}

	// If no suitable position found, add it to the end of the queue
	list_add_tail(&rq->queuelist, &nd->queue);
	printk(KERN_INFO "[SSTF] Added sector %llu to the end (current head pos: %llu)\n",
	       blk_rq_pos(rq), nd->head_pos);
}

/* Initialize the SSTF queue. */
static int sstf_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct sstf_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&nd->queue);
	nd->head_pos = 0;  // Start head position at 0 (or wherever the disk head starts)
	eq->elevator_data = nd;

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);

	return 0;
}

/* Cleanup the SSTF queue. */
static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

/* SSTF elevator type definition. */
static struct elevator_type elevator_sstf = {
	.ops.sq = {
		.elevator_merge_req_fn		= sstf_merged_requests,
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_init_fn		= sstf_init_queue,
		.elevator_exit_fn		= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

/* Initialize the SSTF module. */
static int __init sstf_init(void)
{
	return elv_register(&elevator_sstf);
}

/* Exit the SSTF module. */
static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);

MODULE_AUTHOR("RNB & VCT");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSTF IO scheduler");
