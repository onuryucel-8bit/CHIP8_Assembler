# CHIP8_Assembler
<table border="1">
        <tr>
            <th>Opcode</th>
            <th>Hex Format</th>
            <th>Comment</th>
        </tr>
        <tr>
            <td>CLS</td>
            <td>0x00E0</td>
            <td>clear screen</td>
        </tr>
        <tr>
            <td>RET</td>
            <td>0x00EE</td>
            <td>pc = stack[stack Pointer]</td>
        </tr>
        <tr>
            <td>JMP</td>
            <td>0x1NNN</td>
            <td>pc = nnn</td>
        </tr>
        <tr>
            <td>CALL</td>
            <td>0x2NNN</td>
            <td>stack[stack Pointer] = pc; stackPointer += 2;</td>
        </tr>
        <tr>
            <td>CE</td>
            <td>0x3XNN</td>
            <td>if Vx == nn : pc += 2;</td>
        </tr>
        <tr>
            <td>CNE</td>
            <td>0x4XNN</td>
            <td>if Vx != nn : pc += 2;</td>
        </tr>
        <tr>
            <td>RE</td>
            <td>0x5XY0</td>
            <td>if Vx == Vy : pc += 2;</td>
        </tr>
        <tr>
            <td>LDC</td>
            <td>0x6XNN</td>
            <td>Vx = nn</td>
        </tr>
        <tr>
            <td>ADDC</td>
            <td>0x7XNN</td>
            <td>Vx = Vx + nn</td>
        </tr>
        <tr>
            <td>MOV</td>
            <td>0x8XY0</td>
            <td>Vx = Vy</td>
        </tr>
        <tr>
            <td>OR</td>
            <td>0x8XY1</td>
            <td>Vx = Vx | Vy</td>
        </tr>
        <tr>
            <td>AND</td>
            <td>0x8XY2</td>
            <td>Vx = Vx & Vy</td>
        </tr>
        <tr>
            <td>XOR</td>
            <td>0x8XY3</td>
            <td>Vx = Vx ^ Vy</td>
        </tr>
        <tr>
            <td>ADDR</td>
            <td>0x8XY4</td>
            <td>if Vx + Vy > 0xff(Vf = 1 ) else( Vf = 0) Vx = Vx + Vy</td>
        </tr>
        <tr>
            <td>SUBX</td>
            <td>0x8XY5</td>
          <td>if Vx > Vy :(Vf = 1 ) else( Vf = 0) Vx = Vx - Vy</td>
        </tr>
        <tr>
            <td>SHR</td>
            <td>0x8XY6</td>
          <td>Vx >>= 1; VF = Vx & 0x01;</td>
        </tr>
        <tr>
            <td>SUBY</td>
            <td>0x8XY7</td>
          <td>if Vy > Vx :(Vf = 1 ) else( Vf = 0) Vx = Vy - Vx</td>
        </tr>
        <tr>
            <td>SHL</td>
            <td>0x8XYE</td>
          <td>Vx <<= 1; VF = Vx & 0x80;</td>
        </tr>
        <tr>
            <td>RNE</td>
            <td>0x9XY0</td>
          <td>if Vx != Vy: PC += 2;</td>
        </tr>
        <tr>
            <td>LDI</td>
            <td>0xANNN</td>
          <td>regIndex = nnn</td>
        </tr>
        <tr>
            <td>JMPZ</td>
            <td>0xBNNN</td>
          <td>pc = V0 + nnn</td>
        </tr>
        <tr>
            <td>RND</td>
            <td>0xCXNN</td>
         <td>Vx = randomNumber & nn </td>
        </tr>
        <tr>
            <td>DRW</td>
            <td>0xDXYN</td>
         <td>draw sprite at (Vx,Vy) with n height</td>
        </tr>
        <tr>
            <td>SKP</td>
            <td>0x9X9E</td>
           <td>if keys[Vx] == 1: pc +=2 </td>
        </tr>
        <tr>
            <td>SKNP</td>
            <td>0x9X9E</td>
        <td>if keys[Vx] == 0: pc += 2</td>
        </tr>
        <tr>
            <td>LDD</td>
            <td>0xFx07</td>
         <td>Vx = delayTimer</td>
        </tr>
        <tr>
            <td>WFK</td>
            <td>0xFx0A</td>
          <td>Vx = wait_for_key()</td>
        </tr>
        <tr>
            <td>LDRD</td>
            <td>0xFx15</td>
          <td>delayTimer = Vx</td>
        </tr>
        <tr>
            <td>LDRS</td>
            <td>0xFx18</td>
                <td>SoundTimer = Vx</td>
        </tr>
        <tr>
            <td>ADDI</td>
            <td>0xFx1E</td>
                <td>regIndex += Vx</td>
        </tr>
        <tr>
            <td>IIR</td>
            <td>0xFx29</td>
        <td>regIndex = Ram[Vx] * 0x05 </td>
        </tr>
        <tr>
            <td>DTB</td>
            <td>0xFx33</td>
        <td>bcd</td>
        </tr>
        <tr>
            <td>STR</td>
            <td>0xFx55</td>
        <td>load V0 ... Vx => RAM </td>
        </tr>
        <tr>
            <td>CFR</td>
            <td>0xFx65</td>
        <td>load RAM => V0 ... Vx</td>
        </tr>
    </table>
