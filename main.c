#include <stdio.h>
#include <stdlib.h>

typedef struct MemoryBlock
{
    void *address;
    size_t size;
    struct MemoryBlock *next;
} MemoryBlock;

typedef struct MemoryManager
{
    MemoryBlock *head;
} MemoryManager;

void *allocateMemory(MemoryManager *manager, size_t size)
{
    MemoryBlock *block = manager->head;
    while (block != NULL)
    {
        if (block->size >= size)
        {
            void *address = block->address;
            if (block->size > size)
            {
                MemoryBlock *newBlock = (MemoryBlock *)malloc(sizeof(MemoryBlock));
                newBlock->address = block->address + size;
                newBlock->size = block->size - size;
                newBlock->next = block->next;
                block->next = newBlock;
                block->size = size;
            }

            if (block == manager->head)
            {
                manager->head = block->next;
            }
            else
            {
                MemoryBlock *prevBlock = manager->head;
                while (prevBlock->next != block)
                {
                    prevBlock = prevBlock->next;
                }
                prevBlock->next = block->next;
            }

            return address;
        }
        block = block->next;
    }

    return NULL;
}

void deallocateMemory(MemoryManager *manager, void *address)
{
    MemoryBlock *block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    block->address = address;
    block->size = 0;
    block->next = manager->head;
    manager->head = block;
}

int main()
{
    MemoryBlock *managerHead = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    managerHead->address = managerHead;
    managerHead->size = sizeof(*managerHead);
    managerHead->next = NULL;
    MemoryManager manager;
    manager.head = managerHead;

    int *block1 = (int *)allocateMemory(&manager, sizeof(int));
    int *block2 = (int *)allocateMemory(&manager, 2 * sizeof(int));

    *block1 = 42;
    *block2 = 100;

    printf("Block 1: %d\n", *block1);
    printf("Block 2: %d\n", *block2);

    deallocateMemory(&manager, block1);

    int *block3 = (int *)allocateMemory(&manager, sizeof(int));
    *block3 = 200;

    printf("Block 3: %d\n", *block3);

    deallocateMemory(&manager, block2);
    deallocateMemory(&manager, block3);

    return 0;
}