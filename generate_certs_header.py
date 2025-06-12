def wrap_pem_file(filename, var_name):
    with open(filename, 'r') as f:
        content = f.read()
    return f'const char* {var_name} = R"EOF(\n{content}\n)EOF";\n\n'

def generate_certs_header(cert_file, key_file, ca_file, output='certs.h'):
    cert_code = wrap_pem_file(cert_file, "AWS_CERT")
    key_code = wrap_pem_file(key_file, "AWS_PRIVATE_KEY")
    ca_code = wrap_pem_file(ca_file, "AWS_CA_ROOT")

    with open(output, 'w') as f:
        f.write("// Auto-generated certs header\n")
        f.write("#pragma once\n\n")
        f.write(cert_code)
        f.write(key_code)
        f.write(ca_code)

    print(f"✅ certs.h successfully created at: {output}")

# Example usage — adjust filenames to your actual paths
generate_certs_header(
    cert_file="Arduino_IoT.cert.pem",
    key_file="Arduino_IoT.private.key",
    ca_file="root-CA.crt"
)

