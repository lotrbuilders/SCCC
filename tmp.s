section .data
global a
a: dd 0
section .data
extern malloc
section .data
	section .text
global main:function (main.end-main)
main:
	lea ecx, [esp+4]
	and esp, -16
	push DWORD[ecx-4]
	push ebp
	mov ebp, esp
	push ecx
	sub esp, 4
	push ebx
	pop eax
	push eax
	add esp,-16
	pop eax
	push eax
	mov eax,4
	push eax
	call malloc
	add esp,20
	push eax
	lea eax,[a]
	mov ebx,eax
	pop eax
	mov [ebx],eax
	pop eax
	push eax
	mov eax,1
	push eax
	mov eax,[a]
	mov ebx,eax
	pop eax
	mov [ebx],eax
	pop eax
	push eax
	mov eax,[a]
	mov eax,[eax]
	mov ebx,[ebp-12]
	mov ecx, DWORD[ebp-4]
	leave
	lea esp, [ecx-4]
	ret
.end:
	section .data
section .data
