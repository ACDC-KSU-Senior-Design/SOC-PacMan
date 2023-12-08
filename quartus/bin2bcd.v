module bin2bcd(
   input [15:0] bin,
   output reg [19:0] bcd
);

integer i;

always @(bin) begin
    bcd = 20'b0;
    for (i = 0; i < 16; i = i + 1) begin
        if (bcd[3:0] >= 5) bcd[3:0] = bcd[3:0] + 4'b0011;
        if (bcd[7:4] >= 5) bcd[7:4] = bcd[7:4] + 4'b0011;
        if (bcd[11:8] >= 5) bcd[11:8] = bcd[11:8] + 4'b0011;
        if (bcd[15:12] >= 5) bcd[15:12] = bcd[15:12] + 4'b0011;
        bcd = {bcd[15:0], bin[15-i]};  // Shift one bit, and shift in proper bit from input
    end
end

endmodule