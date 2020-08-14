extern puts
extern malloc
global main:function (main.end-main)
main:
	push ebp
	mov ebp,esp
	pop eax
	push eax
	mov eax,.string0
	push eax
	pop eax
	push eax
	pop eax
	push eax
	mov eax,[ebp-4]
	push eax
	call puts
	add esp,4
	mov esp,ebp
	pop ebp
	ret
.end:
.string0: db 'Hello world',0
