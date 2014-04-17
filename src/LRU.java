public class LRU
{
	public Node head;
	public int maxSize, currentSize;

	public LRU(int size)
	{
		this.maxSize = size;
	}

	public Node initNode(int data)
	{
		Node node = new Node();
		node.value = data;
		node.next = null;
		return node;
	}

	public String report()
	{
		Node current = head;
		String printString = new String();

		if (current == null)
			printString = "";

		while (current != null)
		{
			printString = printString + current.value + " ";
			current = current.next;
		}

		return printString;
	}

	public Node push(Node node)
	{
		if (this.contains(node.value))
			return null;

		Node current = head;
		Node insertNode = node;

		head = insertNode;
		insertNode.next = current;

		currentSize++;

		if (currentSize > maxSize)
		{
			return this.remove(currentSize - 1);
		}

		return null;
	}

	public Node remove(int offset)
	{
		Node current = head;
		int index = 1;

		if (offset == 0)
		{
			head = head.next;
			return current;
		}
		else
		{
			while (current != null)
			{
				if (offset == index)
				{
					Node removed = current.next;
					Node nextNode = current.next.next;
					current.next = nextNode;

					currentSize--;

					return removed;
				}
				else
				{
					current = current.next;
					index++;
				}
			}
		}

		return null;
	}

	public boolean contains(int data)
	{
		Node current = head;
		Node searchNode = initNode(data);

		while (current != null)
		{
			if (current.value == searchNode.value)
				return true;
			else
				current = current.next;
		}

		return false;
	}

	private class Node
	{
		public int value;
		public Node next;
	}

}
