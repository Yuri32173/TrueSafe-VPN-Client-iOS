sub OPENSSL_cpuid_setup {
    return;
}

sub OPENSSL_wipe_cpu {
    my @regs = (0) x 28;
    my @fregs = (0) x 32;

    return;
}

sub OPENSSL_atomic_add {
    my ($ptr, $amount) = @_;

    while (1) {
        my $value = atomic_load($ptr);
        my $new_value = $value + $amount;
        last if atomic_compare_exchange($ptr, $value, $new_value);
    }

    return;
}

sub OPENSSL_rdtsc {
    my $timestamp = time();
    return $timestamp;
}

sub OPENSSL_cleanse {
    my ($data, $length) = @_;

    if ($length > 0) {
        my $buffer = "\x00" x $length;
        $data = $buffer;
    }

    return;
}
